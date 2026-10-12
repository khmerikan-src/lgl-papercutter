#pragma once

#include "core/theme.h"

#include <giomm/filemonitor.h>
#include <giomm/settings.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/window.h>
#include <sigc++/sigc++.h>

#include <map>
#include <string>

namespace papercutter {

// Re-themes the app live when the desktop theme changes. Port of lgl-distro-desktop-score
// app/desktop.py (_watch_theme, _watch_theme_files, _queue_theme, _read_theme):
// Gtk::Settings and org.gnome.desktop.interface changes, plus file monitors on the folders
// of ~/.config/gtk-4.0/gtk.css and its @imports, debounced by 200 ms. On a change the user
// gtk.css is re-read (GTK loads it only once), the pc_* palette and monospace font
// providers are regenerated, and the window's .dark class follows the background.
class ThemeWatcher final : public sigc::trackable {
public:
    explicit ThemeWatcher(Gtk::Window &window);

    const theme::Theme &current() const { return m_theme; }
    std::string monoFamily() const;

    // Emitted after a change has been applied.
    sigc::signal<void()> &signalChanged() { return m_changed; }

private:
    void watch();
    void watchFiles();
    void queue();
    bool changed();
    void apply(bool reloadUserCss);
    theme::Theme read() const;
    std::string interfaceString(const char *key) const;

    Gtk::Window &m_window;
    theme::Theme m_theme;
    Glib::RefPtr<Gtk::CssProvider> m_userProvider;
    Glib::RefPtr<Gtk::CssProvider> m_paletteProvider;
    Glib::RefPtr<Gtk::CssProvider> m_fontProvider;
    Glib::RefPtr<Gio::Settings> m_interface;
    std::map<std::string, Glib::RefPtr<Gio::FileMonitor>> m_monitors;
    sigc::connection m_timeout;
    sigc::signal<void()> m_changed;
};

// Adds data/style.css at APPLICATION priority for the default display.
void installApplicationStylesheet();

} // namespace papercutter
