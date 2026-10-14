<div align="center">

# LGL Papercutter

**A simple way to modify wallpapers to make them a better fit for your desktop.**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Release](https://img.shields.io/badge/release-0.4.1-purple)](CHANGELOG.md)
[![Arch](https://img.shields.io/badge/Arch%20Linux-179ae8?logo=archlinux&logoColor=white)](https://wiki.archlinux.org/)
[![AppImage](https://img.shields.io/badge/AppImage-available-brightgreen?logo=appimage&logoColor=white)](https://github.com/linuxgamerlife/lgl-papercutter/releases)
[![RPM](https://img.shields.io/badge/RPM-available-brightgreen?logo=redhat&logoColor=white)](https://github.com/linuxgamerlife/lgl-papercutter/releases)
[![GTK](https://img.shields.io/badge/GTK-4-green?logo=gtk&logoColor=white)](https://www.gtk.org)
[![Support on Ko-fi](https://img.shields.io/badge/Support_on-Ko--fi-FF5E5B?logo=ko-fi&logoColor=white)](https://ko-fi.com/linuxgamerlife)

</div>

---

## Overview

LGL Papercutter is a graphical wallpaper editor for Linux. Add your existing
images, choose your monitor resolution, and visually position each wallpaper
before saving it at exactly the right size.

- Drag, zoom, and preview the final crop before changing any file
- Use detected display sizes, common presets, or a custom resolution
- Process portrait, landscape, and ultrawide wallpapers without distortion
- Keep everything private and local — images are never uploaded
- Match your desktop: Papercutter follows your GTK theme, colours, and fonts live
- Export processed copies without changing the source images

---

## How It Works

1. Add one or more JPEG, PNG, or WebP images using **Add Images** or drag and drop.
2. Select your target resolution. Select multiple queue entries first to apply
   the resolution to all of them at once.
3. Drag the current image to position it and adjust the zoom until the preview
   looks right. Each image retains its own staged resolution, position, and
   zoom while you move through the queue.
4. Choose **Save As** to preview the exact output names and locations, then
   export the processed wallpapers as new files.

Papercutter never replaces a source during export. It remembers the last folder
you saved to and asks before replacing an existing destination. Queue removal
does not touch source files; the separate Move to Trash action always asks for
confirmation.

---

## Features

| Feature | What it does |
|---|---|
| **Follows your theme** | Uses your GTK palette, light or dark mode, accent colour, and fonts, and updates live when they change |
| **Visual composition** | Preview the exact target shape and reposition the image by dragging |
| **Zoom control** | Choose how tightly the wallpaper is cropped |
| **Output quality** | Reduce JPEG and WebP file sizes and preview the estimated output size |
| **Resolution targets** | Use detected displays (the current one first), presets, or custom width and height |
| **Staged compositions** | Retain an independent resolution, crop, position, and zoom for every queued image |
| **Image attributes** | View and edit Description, Artist, Copyright, and Comment metadata per image |
| **Multi-image resolution changes** | Apply a target resolution to any selection or the entire queue |
| **Portrait and ultrawide support** | Compose wallpapers for horizontal or vertical displays |
| **Image queue** | Review several wallpapers in one session |
| **Batch export** | Preview and save several composed images into one chosen folder |
| **Numbered outputs** | Optionally continue from the highest numbered filename already in the destination |
| **Drag and drop** | Drop image files or a folder directly into the window |
| **File actions** | Right-click selected queue items to save, export, remove, or move sources to Trash |
| **Duplicate removal** | Find duplicate queued images by their contents |
| **Save As** | Export a separate copy without changing the source |
| **Local processing** | Process images with ImageMagick without accounts, uploads, or telemetry |
| **Built-in help** | Explain every control, staged-state behaviour, export logic, and file-safety rule inside the app |
| **About** | Show the version, project and author links, licence, and Ko-fi support link |

---

## Install

The current release is **0.4.1**.

This fork targets **Arch Linux / CachyOS (Arch-based)**. The upstream project
was released for Fedora; this fork keeps the same GTK 4 / libadwaita
application and rebuilds it for Arch-based systems.

### Recommended — prebuilt AppImage

Download the AppImage from [GitHub Releases](https://github.com/linuxgamerlife/lgl-papercutter/releases)
and run it. No system dependencies are installed and no package management is
required.

### Build from source

Install the build dependencies first:

```bash
sudo pacman -S cmake gcc pkgconf gtkmm-4.0 libadwaita imagemagick exiv2
```

Then, from the repository root, follow the [Build from Source](#build-from-source) section below.



---

## What's New in 0.4.1

- Optional system title bar: press `Ctrl+Shift+M` or choose **Use System Title
  Bar** in the main menu to let your compositor draw the title bar; the header
  bar then sits inside the window. The choice is remembered.
- Tap `Alt` to open or close the main menu
- The app icon is installed at 48, 64, 128 and 256 px
- Source builds now start from the repository root

## What's New in 0.4.0

- Migrated to GTK 4 for better theming and a better look on GTK desktops
- Follows your desktop GTK theme live: colour palette (including Noctalia),
  light or dark mode, accent colour, and fonts
- Redesigned window with the app icon, queue thumbnails, and settings cards
- The display the window opens on is the default target resolution
- Rotated JPEG photos no longer distort in batch exports, and large folders
  load without freezing the window

See [CHANGELOG.md](CHANGELOG.md) for the complete release history.

---

## Build from Source

Papercutter is a GTK 4 and libadwaita application in the `gtk/` folder, built from the
repository root. It
requires gtkmm 4.20 and libadwaita 1.6 or newer, and supports both ImageMagick 7's `magick` command and ImageMagick 6's
`convert` command. Exiv2 is used for image attributes.

Install the dependencies listed in the [Install](#install) section, then clone, build, and test:

```bash
git clone https://github.com/linuxgamerlife/lgl-papercutter.git
cd lgl-papercutter
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
```

Run it directly from the build directory:

```bash
./build/lgl-papercutter
```

Ubuntu 24.04 and Linux Mint 22 ship older GTK libraries and cannot build 0.4.x from
source. The previous Qt version (0.3.1) remains available from
[GitHub Releases](https://github.com/linuxgamerlife/lgl-papercutter/releases).

---

## File Safety

Save As renders into a temporary file, validates the requested dimensions, and
then commits the new destination. It refuses to use the source path as the
destination, so exporting cannot replace the original image.

Removing an entry from the queue does not change the source file. **Move to
Trash** is the only source-file mutation and always requires confirmation.

---

## License

MIT — see [LICENSE](LICENSE) for details.

---

<div align="center">
Made for <a href="https://wiki.archlinux.org/">Arch Linux</a> · by <a href="https://www.youtube.com/@linuxgamerlife">LinuxGamerLife</a>
</div>
