# LGL Papercutter Help

LGL Papercutter prepares wallpaper copies for a chosen screen resolution. Editing is staged inside the app: your source images are unchanged until you explicitly use **Move to Trash**.

Papercutter follows your desktop's GTK theme, including light or dark mode, the accent colour, and the monospace font. Theme changes apply while the app is open.

## Main controls

The header bar holds the main actions:

- **Add Images** opens your desktop file picker and adds JPEG, PNG, or WebP images to the queue. You can also drag images or folders onto the window. Folders add the supported images directly inside them, not in subfolders.
- **Remove** removes the selected entries from the queue without deleting their source files. Pressing the **Delete** key while the queue has focus does the same; it only removes queue entries and does not delete images from your computer.
- **Remove Duplicates** compares image contents and removes later duplicate entries from the queue. It does not delete files.
- **Save As…** exports the selected staged image or images as new files.

The main menu (the button at the right of the header bar) also provides **Add Folder…**, **Select All**, **Remove Duplicates**, **Save As…**, **Help**, **About LGL Papercutter**, and **Quit**.

Keyboard shortcuts:

- `Ctrl+O` — Add Images
- `Ctrl+Shift+O` — Add Folder
- `Ctrl+Shift+S` — Save As
- `Ctrl+A` — Select all queue entries (while the queue has focus)
- `Delete` — Remove the selected entries from the queue (while the queue has focus)
- `F1` — Help
- `Ctrl+Q` — Quit

Short status messages appear at the bottom of the window and disappear on their own.

## Queue and selection

Click an image to make it the current image. Use `Ctrl` or `Shift` while clicking to select several images, or use **Select All** from the main menu.

Each queue entry shows a thumbnail, the filename, and its staged target resolution. Each queued image keeps its own staged target resolution, zoom, position, output quality, and image attributes. Switching to another image does not discard those changes.

Right-click a queued image for:

- **Save As…** — export the selected image or images.
- **Move to Trash…** — move the original source files to the desktop Trash after confirmation. This is the only queue command that changes the source files.
- **Remove from Queue** — stop working with the selected entries without touching their files.

## Composition controls

- **Target resolution** chooses a detected display size or saved preset. The display the window opens on is listed first and selected by default.
- The **width × height** boxes set a custom output size.
- Changing the target resolution while several images are selected applies it to all selected images. If selected images currently use different sizes, the menu shows **Mixed resolutions**.
- **Zoom** changes only the current image. Drag the image on the canvas to position its crop. You can also scroll over the canvas to zoom.
- **Reset** returns the zoom to 100%.
- **Effective scale** shows how much the current image is enlarged or reduced. **SHARP** means the image is shown at or below its original size; **ENLARGED** and **SOFT** warn that enlargement may make the result look soft.
- **Output quality** uses a slider to control JPEG and WebP compression for the current image; **Reset** returns it to 95%. The heading identifies the active format. For PNG sources it shows **PNG compression** and disables the quality slider because PNG size is unaffected by that setting. Indexed PNG sources retain palette compression instead of being expanded into much larger true-colour files. The size display shows the original file size and asynchronously renders an estimated output for the current format, crop, resolution, and quality.
- **Image attributes** shows the selected image's Description, Artist, Copyright, and Comment metadata. Edits remain staged with that image and are written to the exported copy. Exiv2 is required to read and write these attributes.

All changes remain staged per image until you export them.

## Saving one image

Select one image and choose **Save As…**. Pick a new filename and location in the desktop file picker. If the destination folder contains numerically named files, Papercutter offers to number the export sequentially, using the first missing number or continuing the sequence, or to keep the chosen name. Papercutter will not overwrite the original source. If another file already uses that destination, you decide whether to replace it.

A confirmation window shows the exact source and destination before the export starts.

## Saving several images

Select multiple queue entries and choose **Save As…**, then choose a destination folder.

If that folder contains files whose complete names before the extension are numbers, Papercutter offers to fill gaps between the lowest and highest numbers before continuing the sequence. For example, files `0312.jpg` and `0314.jpg` make the next export `0313`; after the gaps are filled, numbering continues at `0315`. Existing zero padding is retained. If no numbered filename exists, or you choose **Keep Names**, each image keeps its current filename.

If a planned file already exists, or two exports would use the same name, choose **Replace**, **Skip**, or **Cancel Batch**.

Before anything is written, the confirmation window lists every source filename beside its planned destination. Drag the divider between the two columns when longer filenames need more room.

Exports are written as new files and validated before completion. Failed or skipped items are listed in a batch report; successful items remain saved.

## File safety summary

- Staging, zooming, cropping, resizing, queue removal, and duplicate removal do not modify source files.
- **Save As…** creates processed copies at destinations you approve.
- **Move to Trash** is the only action that removes source files, and it asks for confirmation first.
