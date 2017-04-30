██████╗ ███████╗ █████╗ ████████╗ ██████╗  ██████╗ ██╗     
██╔══██╗██╔════╝██╔══██╗╚══██╔══╝██╔═══██╗██╔═══██╗██║     
██████╔╝███████╗███████║   ██║   ██║   ██║██║   ██║██║     
██╔══██╗╚════██║██╔══██║   ██║   ██║   ██║██║   ██║██║     
██████╔╝███████║██║  ██║   ██║   ╚██████╔╝╚██████╔╝███████╗
╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝V1.1

# Summary

BSATool is part of the ArenaToolBox project which want to provide translators and modders various tools for creating, editing and update the different TES1: Arena file formats.
BSATool provides support for exploring, editing and creating Bethesda Arena BSA. While some other game of Bethesda of the same period can be edited with this tool, the support is optimized for Arena and results are not guaranteed for other game. Also, keep in mind that this tool is in under development and has not yet all his functionalities.
BSATool was created by David Aussourd alias Dysperia (softwatermermaid@hotmail.fr) with Qt 5 under Qt creator 3.

# ArenaToolBox parts :
* [BSATool](https://github.com/Dysperia/ArenaToolBox-BSATool "BSATool")
* [TemplateEditor (old version)](https://github.com/Dysperia/TemplateEditor "TemplateEditor")
* [TemplateEditor (new version, currently on development)](https://github.com/Dysperia/ArenaToolBox-TemplateEditor "TemplateEditor")

# Various links
## French translation project (PFA)
[PFA Wiki](http://www.projet-french-arena.org/wiki/ "PFA Wiki") - [PFA forum](http://www.projet-french-arena.org/forum/ "PFA Forum")

## ArenaToolBox download from the PFA
Be aware that the template editor is in development and some bugs have been found while using it in Turkey. The BSATool should be working for everyone.
[ArenaToolBox](http://www.projet-french-arena.org/files/ArenaToolBox_win32bit.zip "ArenaToolBox")

# Historic
________________________V1.1b_______________________

as of september 04, 2014

* Support for the 09 palette flag of IMGs. All CHARBKxx.IMG and CHRBKFxx.IMG use now their own integrated palette instead of CHARSHT.COL (both being the same)


________________________V1.1a_______________________

as of august 22, 2014

* Last opened BSA directory is saved to be proposed next time user load a BSA.
* First color of palette in preview and png is set to alpha 1 and not 0. While this is not visible by the user, it makes all the 256 colors to be visible in image editing program like Gimp.


________________________V1.1________________________

as of august 15, 2014

* Crypt/decryption of external inf file(s). To extract and decrypt, just extract as raw and use the external decrypt/crypt Function.
the algorithm used is the one Arena use, not the one from winArena (Arena one is faster and simpler).
* INF file can be previewed to see if there are well decrypted in Arena.
* Extended preview support has been added for INF file in order to preview the whole file.
* Added choice for texture IMG (64x64) when chosing how to convert non native PNG to IMG/SET
* Some minor bugs corrected:
    Ask for choice if unsaved change when user close the app by clicking the red cross too (only available by the exit menu before)
    Update the filter box too when updating the file list after user added files or after bsa reloading due to save.


________________________V1.0________________________

As of august 11, 2014

* New bsa creation.
* Save current changes to a bsa file (new or not).
* Exploration and edition of an existing bsa file.

* Add/delete/update file(s) to archive. Be aware than the maximum length for a file name due to BSA file format limitation is 13 characters. BSATool will check it!
* Multiple operations can be set on a single file, this not a bug. BSATool will then take priority for deletion then updated then new while saving.
* Cancel of delete/update operation. To cancel new file, just delete it.
* State of file are shown in info and file list. The color code of a state follow the one used for the corresponding operation icon.

* Some info on BSA are displayed. The number between round brackets after the size is the difference between opened/created bsa and the saved one with all current changes applied.
* Info on file will vary depending on the currently selected file. For now, special information are displayed for IMG/SET, only basic for others.

* Full support for IMG/SET files.
* BSATool has knowledge of the 947 native IMGs with their type, color palette and file header. It will help to take decision while converting png to IMG.
* All the IMG are accessible as the 2 decompression algorithms used have been founded (after about 1-2 months of work but founded!).
* BSATool has knowledge of the 180 SETs native of Arena along with their dimensions, (also help to convert png).
* Preview for IMG/SET file.
* Extended preview in view menu will allow you to change between color palette and see the changes. However, I tried my best to set a default palette for all native IMG/SET, in order to see what Arena should display (be aware than some IMG will use various palette depending on the call in game, for example DUNGEON.IMG will use the different integrated palette of each different province).
This functionality was mostly designed to help modding of the game if a new engine is created and new bsa/files can be used in game. The toolbox which BSATool is the only part, aims also to see if all file format can be decoded and understood. After than only an engine recreation can be started by some crazy guys.
* Choice in view menu to set the preview on archive file or the update one. This was written to see the changes and check file before saving.
* External preview of IMG/SET file.
* BSATool has access to all the 4 native COL files and 39 integrated palette in IMGs. You can switch between all of them in external/extended preview but be aware than some can be identical or very similar (for example, all province palette are close to each other).
* The palette used by the actual IMG/SET in preview can be seen.
* The first color of the color palette is set to be displayed transparent has it is the case in Arena (Alpha channel does not exist). PNG created will have the same setting but IMG/SET created from them lose the transparency and keep only the 3 color channels. This only for display and easy editing.

* Raw extraction of one or all filtered file(s).
* Extraction with decompression of one or all IMG/SET.
* Extraction with conversion to PNG of one or all IMG/SET.
* External decompression of IMG(s). decompression algorithm is chosen upon header flag and not filename. Not native IMGs with the same format can also, in theory, be uncompressed.
* External conversion of IMG/SET file(s) to PNG. If file is not native and have no palette integrated, BSATool will ask the user to choose a palette to include in the PNG file or to use default PAL.COL. Not native IMG have to be in a valid format (see below for information about Arena IMG/SET file).
* External conversion of PNG(s) to IMG(s)/SET(s). Native IMG/SET will be setting up automatically by checking the filename and taking decision from it. If the image dimension does not match the native file, ask to user will be made to know if he really want to make the conversion, as this could break compatibility with Arena. If the filename is not known in IMG/SET file database, a window will be prompted to ask the user to choose to which format and with which settings he wants to convert the PNG file.


___________________BSA File Format__________________

The first 2 bytes of the file give the total number of file stored in archive.
After follow each file stored one after an other.
Then, after the last file, is the file table. Each record is composed of a 14 bytes filename ending in 0x00 (filling with 0x00 byte if the name is shorter than 13 characters) followed by a 4 bytes file size. Each record is then 18 bytes long (with the same number of entry than the number of file of course!). Then the offset of a file in the archive is 2 (the header) + the sum of the size of all the files preceding the one we are looking for.

Integrity of archive is checked by BSATool by summing all the file size, the header (2 bytes) and 18*fileNumber (length of the file Table). If this number is equal to the total archive size, the BSA pass the checking process with success. After saving changes, the saved BSA is reloaded to check its integrity.


___________________SET File Format__________________

Set are 64x(n*64) image file used to store textures. Width is always 64 and since it stores various 64x64 texture in a vertical way, height is a multiple of 64. When editing and creating SET file for Arena, keep in mind to not change the dimensions of the file, otherwise Arena could not display it correctly (BSATool will help while converting from png by warning you about a difference in dimensions). That's all about it, there is no header, no palette. Bytes (which are indexes in PAL.COL) are written in this file and is size is directly width*height.


___________________IMG File Format__________________

IMF files are not easy to deal with. If the IMG file is 64x64 (then size of 4096) it is a texture and then have no header. It is more like a SET file with only one texture. Beside that, some IMG have no header and dimensions to set manually. The list is given below:

 filename         width       height

ARENARW.IMG         16          16
CITY.IMG            16          11
DITHER.IMG          16          50
DITHER2.IMG         16          50
DUNGEON.IMG         14           8
DZTTAV.IMG          32          34
NOCAMP.IMG          25          19
NOSPELL.IMG         25          19
P1.IMG             320          53
POPTALK.IMG        320          77
S2.IMG             320          36
SLIDER.IMG         289           7
TOWN.IMG             9          10
UPDOWN.IMG           8          16
VILLAGE.IMG          8           8

All other IMG have a header of 12 bytes followed by the image data, followed by the palette if one is integrated. The header is composed in order of :
offsetX     (2 bytes)
offsetY     (2 bytes)
width       (2 bytes)
height      (2 bytes)
flags       (2 bytes)
dataSize    (2 bytes)

the flags are composed of 2 flag:
the compression flag is 0x00/0x04/0x08 (0x00 no compression, 0x04 and 0x08 compression 4 and 8)
the palette flag is not all known. 0x01 means integrated palette but the meaning of 0x00, 0x08 and 0x09 is unknown.

When opening or converting a none native Arena IMG, BSATool will expect the file to have a header and not be a special IMG. When converting IMG from png, if the filename is not known and user choose IMG format, BSATool will create a normal uncompressed IMG with header. The palette flag is set to 0x00 or 0x01 according to the choice the user made to include or not the png color palette in the IMG file.
