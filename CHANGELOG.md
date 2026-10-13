# Changelog

All notable user-facing changes to LGL Papercutter are recorded here.

## 0.4.1 — 2026-10-13

### Added

- **Use System Title Bar** in the main menu (`Ctrl+Shift+M`) lets the
  compositor draw the title bar; the header bar, with all its buttons and the
  main menu, then sits inside the window. The choice is saved in
  `~/.config/lgl-papercutter/settings.ini`.
- Tapping `Alt` opens and closes the main menu.

### Changed

- Papercutter now follows the LGL GTK Desktop Application Standard shared with
  LGL Emoji Picker.
- The app icon is installed at 48, 64, 128 and 256 px instead of only 512 px.
- The repository root builds the GTK app (`cmake -B build`); the Qt sources are
  kept for reference only and are no longer built.
- The Save As confirmation window has its own header bar, and the Save As
  button tooltip names its shortcut.

## 0.4.0 — 2026-10-12

### Changed

- Migrated from Qt to GTK 4 (gtkmm) and libadwaita for better theming and a
  better look on GTK desktops. The packaged application is now built from `gtk/`.
- Papercutter follows the desktop GTK theme live: the `gtk.css` colour palette
  and its imports (such as Noctalia-generated palettes), light or dark mode,
  the accent colour, and the monospace font. The palette applies to every
  control whichever GTK theme is installed, and dark mode and the accent colour
  come from the desktop, so stock GNOME and KDE setups work without a custom
  `gtk.css`. Changes apply without restarting.
- Redesigned the window: a header bar with the app icon and main actions, a
  queue with thumbnails and staged resolutions, and a sidebar of Target
  resolution, Composition, Output, and Image attributes cards.
- The default target resolution is now the display the window opens on, since
  GTK 4 has no primary-monitor setting.
- Replaced the menu bar and its **Show Menu Bar** toggle with a main menu in
  the header bar. Status messages appear briefly at the bottom of the window.
- The Effective scale read-out shows a **SHARP**, **ENLARGED**, or **SOFT** label.
- File numbering is now offered with **Number Sequentially** and **Keep Names**
  buttons.
- Settings are stored in `~/.config/lgl-papercutter/settings.ini`, so the last
  used source and Save As folders start fresh.
- Source builds now require gtkmm 4.20 and libadwaita 1.6 or newer (Fedora 44).

### Added

- Keyboard shortcuts for Add Folder (`Ctrl+Shift+O`) and Save As
  (`Ctrl+Shift+S`).
- Image files and folders passed on the command line, and images opened with
  **Open With** in a file manager, are added to the queue.

### Fixed

- Rotated JPEG photos are no longer distorted when they are batch exported
  without first being opened in the canvas.
- Large folders no longer freeze the window while thumbnails and previews load.

## 0.3.1 — 2026-10-12

### Added

- A **Reset** button beside the zoom slider to return the zoom to 100%.
- A 5120 × 1440 super-ultrawide resolution preset.
- Gap-aware sequential filenames for single-image and batch exports.
- Editable Description, Artist, Copyright, and Comment image attributes that
  remain staged per image and are written to exported copies.
- Per-image JPEG and WebP output-quality controls with live source and estimated
  output file sizes.

### Fixed

- Preserved palette compression for indexed PNG sources instead of expanding
  slightly scaled images into very large true-colour PNG files.
- Selected and labelled the system primary monitor as the default target, and
  refreshed that default when the primary monitor changes.
- Allowed source builds with Qt 6.4, including Ubuntu 24.04 and Linux Mint 22.
- Added support for ImageMagick 6's `convert` command while retaining support
  for ImageMagick 7's `magick` command.

## 0.3.0 — 2026-08-24

### Added

- Comprehensive in-app Help explaining toolbar buttons, queue selection,
  composition controls, staged per-image state, single and batch Save As,
  numbered output planning, and file-safety behaviour.
- An About dialog with the Papercutter icon, current application version,
  project and issue-tracker link, LinuxGamerLife link, MIT licence information,
  and Ko-fi support link.
- Help and About actions in both the Help menu and the always-visible More menu.

### Changed

- Clarified that the Delete key is a shortcut for removing selected queue
  entries and does not delete source images from the computer.
- Included the standalone help document in source archives and RPM documentation.

## 0.2.0 — 2026-08-24

### Added

- Multi-selection and Select All support for applying a target resolution to
  several queued images at once.
- Independent staged resolution, crop, position, and zoom state for every
  queued image.
- A mixed-resolution state when selected images have different staged targets.
- Batch Save As with an itemised source-to-destination preview.
- Optional sequential output numbering based on the highest numbered file in
  the selected destination folder.
- Right-click queue actions for Save As, Remove from Queue, and Move to Trash.

### Changed

- Replaced the source-overwriting Accept & Save workflow with non-destructive
  Save As exports.
- Improved export confirmation with a resizable, scrollable two-column file
  preview and adjustable column divider.
- Improved integration with KDE theming, desktop portals, and native file
  dialogs when installed as a package.
- Preserved each selected image's extension during numbered batch exports.

### Removed

- Accept & Save, source replacement, backup creation, backup history, and
  restoration controls.
- The redundant toolbar save icon.

## 0.1.0 — 2026-08-20

- Initial Fedora development release with visual wallpaper composition,
  display-resolution targets, zoom and positioning controls, and local
  ImageMagick processing.
