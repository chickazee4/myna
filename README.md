![myna logo](https://raw.githubusercontent.com/chickazee4/myna/main/gfx/logo_master.svg)

myna is a free and open source C++ and wxWidgets-based alternative to the [Starling/Star32 software](https://starlingdb.org/downl.php?lan=en#soft) for viewing Sergei Starostin's (and others') [etymological databases](https://starling.rinet.ru/downl.php), providing a lightweight (i.e., very rudimentary), Unix-compatible graphical interface for browsing them as well as functionality for exporting them as .csv format spreadsheets.

# Acknowledgments

As in my other Starling-related repository, I want to thank Rob Verhoeven and Dr. Andries Brouwer for the work they performed to produce a specification of the Starling *.dbf format, which was integral to the development of libstarling. If you are looking for a Windows tool that accomplishes the same things as the Unix-based myna (and more, I believe — I haven't used it or looked at its code myself), perhaps see [their implementation](https://github.com/rhaver/starling).

CC attribution: I modeled the myna icon on [this image](https://commons.wikimedia.org/wiki/File:Acridotheres_tristis_-Sydney,_Australia-8.jpg) of *Acridotheres tristis* from Richard Taylor/Wikimedia Commons.

# Building/installing

Build-time prerequisites (on Linux):

* C/C++ compiler (must be compatible with C++17) and libc

* [CMake](https://cmake.org) (version >= 3.24)

* [wxWidgets](https://www.wxwidgets.org) (version >= 3.0)

* [libstarling](https://github.com/chickazee4/starling) (version >= 0.3.0; optional — the build system can obtain & link statically)

myna uses a CMake-based build system, so it can be obtained and built as follows:

    $ git clone https://github.com/chickazee4/myna
    $ mkdir myna/build && cd myna/build
    $ cmake .. [-DREMOTE_LIBSTARLING=ON]
    $ make
    # make install

# Usage

Since it is in a very early stage, the myna interface currently is quite rudimentary. If you are comfortable with the terminal, [starling2csv](https://github.com/chickazee4/starling) incorporates more of libstarling's abilities and provides more flexible control over output format. The only ability myna has that starling2csv does not is table editing before export (myna allows you to delete rows/columns and modify individual cells if you desire; starling2csv could not reasonably provide this, since it is not interactive).

Since myna's interface is currently so basic, most aspects should be fairly self-explanatory. The main window consists solely of a table view and a menu bar. The table view will include the contents of a database when loaded and be blank otherwise. The menu bar contains the following entries:

* File
  - Import .dbf — Will display a file chooser dialog with which you can choose a database to load. .var and .inf files will be automatically detected. If no .var file exists, a warning will be shown to indicate that this may cause crashes if the database expects a .var. No message will be displayed if there is no .inf file, since it is not required to display or export a database.
  - Export .csv — Will display a file chooser dialog with which you can choose a location to save a .csv file corresponding to the database. When saved, the contents of the table view **as shown** (changes made to the view since loading will be reflected!) will be saved to a **comma-delimited** UTF-8 .csv file.
  - Quit — Quit the program. If the data has been modified, a prompt will be shown to confirm close.
* Edit
  - Delete row/column — Deletes a selected row/column. You need to click on the row/column number to select the whole row/column in order to delete it due to certain limitations of wxWidgets (you can't select just a single cell).
  - Insert row/column — Adds a row/column at the bottom/right of the table respectively.
  - Sort ascending/descending — Currently does nothing (functionality will be added in the future)
* View
  - Show deleted entries — Sometimes, Starling rows have a flag indicating that they are "deleted" and should not be shown. If this is checked, they will be shown anyway. Whether these entries are included in exported csvs depends on whether this is checked.
  - Show indices column — Show a column indicating row index. If this is checked, indices will also be included in exported csvs.
  - Use human-readable column names — Use human-readable column aliases from a database's .inf file rather than the internal column names, which are often truncated/linguistically confusing (for example, Georgian is sometimes indicated internally with the sequence GRU, which makes sense because in Russian, it is called грузинский **gruzinskii*, but is not transparent to a speaker of another language). This only makes a difference if myna is able to find the inf file for the database.
  - Show database metadata... — Opens a dialog containing metadata for the database such as the date it was last modified, the number of records and fields, and a description of the database from the .inf file, if available.

# Future features

(in order of greatest to least priority)

* Fix: Random bus errors occur about a quarter the time when opening a database. Seems to originate from a threading issue? Traceback indicates it results from libstarling, but nothing similar ever happens in starling2csv.
* Undo/redo
* Sort data by column
* Successful macOS compile + run
* Change output csv delimiter (not just commas)
* Status bar
* Recent files dropdown
* Wide table like `starling2csv -l` (columns become rows and rows become columns)
* Dialog/tab to display PRT data
* Use the Starling format tags for rich text formatting in the viewer instead of ignoring them? Not sure about feasibility