#include "ui/themewatcher.h"

#include <giomm/file.h>
#include <giomm/settingsschemasource.h>
#include <glibmm/fileutils.h>
#include <glibmm/main.h>
#include <glibmm/miscutils.h>
#include <gtkmm/settings.h>
#include <gtkmm/styleprovider.h>

#include <algorithm>
#include <cctype>

namespace papercutter {

namespace {

constexpr unsigned int debounceMs = 200;

Glib::RefPtr<Gtk::CssProvider> createProvider(const unsigned int priority)
{
    auto provider = Gtk::CssProvider::create();
    // The user's gtk.css commonly @imports files that do not exist (libadwaita.css);
    // GTK already reports those once at startup.
    provider->signal_parsing_error().connect(
        [](const Glib::RefPtr<const Gtk::CssSection> &, const Glib::Error &) {});
    Gtk::StyleProvider::add_provider_for_display(Gdk::Display::get_default(), provider,
                                                 priority);
    return provider;
}

bool endsWith(const std::string &text, const std::string &suffix)
{
    return text.size() >= suffix.size()
        && text.compare(text.size() - suffix.size(), suffix.size(), suffix) == 0;
}

} // namespace

void installApplicationStylesheet()
{
    auto provider = Gtk::CssProvider::create();
    provider->signal_parsing_error().connect(
        [](const Glib::RefPtr<const Gtk::CssSection> &section, const Glib::Error &error) {
            g_warning("style.css:%u: %s", section ? static_cast<unsigned>(section->get_start_location().get_lines() + 1) : 0u,
                      error.what());
        });
    provider->load_from_resource("/com/linuxgamerlife/papercutter/style.css");
    Gtk::StyleProvider::add_provider_for_display(Gdk::Display::get_default(), provider,
                                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

ThemeWatcher::ThemeWatcher(Gtk::Window &window)
    : m_window(window)
    , m_userProvider(createProvider(GTK_STYLE_PROVIDER_PRIORITY_USER))
    , m_paletteProvider(createProvider(GTK_STYLE_PROVIDER_PRIORITY_APPLICATION + 1))
    , m_fontProvider(createProvider(GTK_STYLE_PROVIDER_PRIORITY_APPLICATION + 2))
{
    watch();
    apply(false);
}

std::string ThemeWatcher::monoFamily() const
{
    return m_theme.monoFont.empty() ? std::string("Adwaita Mono") : m_theme.monoFont;
}

void ThemeWatcher::watch()
{
    auto settings = Gtk::Settings::get_default();
    for (const char *name : {"gtk-application-prefer-dark-theme", "gtk-interface-color-scheme",
                             "gtk-theme-name", "gtk-font-name"}) {
        if (g_object_class_find_property(G_OBJECT_GET_CLASS(settings->gobj()), name))
            settings->connect_property_changed(name, sigc::mem_fun(*this, &ThemeWatcher::queue));
    }
    auto source = Gio::SettingsSchemaSource::get_default();
    if (source && source->lookup("org.gnome.desktop.interface", true)) {
        m_interface = Gio::Settings::create("org.gnome.desktop.interface");
        m_interface->signal_changed().connect([this](const Glib::ustring &) { queue(); });
    }
    watchFiles();
}

void ThemeWatcher::watchFiles()
{
    const std::string config = Glib::get_user_config_dir();
    std::vector<std::string> folders{Glib::build_filename(config, "gtk-4.0")};
    for (const std::string &path : theme::gtkCssPaths(config))
        folders.push_back(Glib::path_get_dirname(path));
    for (const std::string &folder : folders) {
        if (m_monitors.count(folder) != 0)
            continue;
        try {
            auto monitor = Gio::File::create_for_path(folder)->monitor_directory(
                Gio::FileMonitorFlags::WATCH_MOVES);
            monitor->signal_changed().connect(
                [this](const Glib::RefPtr<Gio::File> &file, const Glib::RefPtr<Gio::File> &other,
                       Gio::FileMonitor::Event) {
                    for (const auto &changed : {file, other}) {
                        if (changed && endsWith(changed->get_basename(), ".css")) {
                            queue();
                            return;
                        }
                    }
                });
            m_monitors[folder] = monitor;
        } catch (const Glib::Error &) {
        }
    }
}

void ThemeWatcher::queue()
{
    // Theme generators write several files in quick succession; apply once.
    m_timeout.disconnect();
    m_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this, &ThemeWatcher::changed),
                                               debounceMs);
}

bool ThemeWatcher::changed()
{
    watchFiles();
    apply(true);
    return false;
}

std::string ThemeWatcher::interfaceString(const char *key) const
{
    if (!m_interface)
        return {};
    auto schema = m_interface->property_settings_schema().get_value();
    if (!schema || !schema->has_key(key))
        return {};
    return m_interface->get_string(key);
}

theme::Theme ThemeWatcher::read() const
{
    auto settings = Gtk::Settings::get_default();
    std::string themeName = settings->property_gtk_theme_name().get_value();
    std::transform(themeName.begin(), themeName.end(), themeName.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    const bool preferDark = interfaceString("color-scheme") == "prefer-dark"
        || settings->property_gtk_application_prefer_dark_theme().get_value()
        || settings->property_gtk_interface_color_scheme().get_value()
            == Gtk::InterfaceColorScheme::DARK
        || endsWith(themeName, "-dark");

    theme::Theme current;
    current.colors = theme::gtkColors(theme::gtkCssPaths(Glib::get_user_config_dir()));
    current.dark = theme::isDark(current.colors, preferDark);
    std::string font = interfaceString("font-name");
    if (font.empty())
        font = settings->property_gtk_font_name().get_value();
    current.font = theme::fontFamily(font);
    current.monoFont = theme::fontFamily(interfaceString("monospace-font-name"));
    current.accentName = interfaceString("accent-color");
    return current;
}

void ThemeWatcher::apply(const bool reloadUserCss)
{
    m_theme = read();
    if (reloadUserCss) {
        const std::string userCss =
            Glib::build_filename(Glib::get_user_config_dir(), "gtk-4.0", "gtk.css");
        if (Glib::file_test(userCss, Glib::FileTest::IS_REGULAR))
            m_userProvider->load_from_path(userCss);
        else
            m_userProvider->load_from_string("");
    }
    m_paletteProvider->load_from_string(theme::paletteStylesheet(m_theme));
    m_fontProvider->load_from_string(theme::fontStylesheet(m_theme));
    if (m_theme.dark)
        m_window.add_css_class("dark");
    else
        m_window.remove_css_class("dark");
    m_changed.emit();
}

} // namespace papercutter
