# v0.2
* Program checks for changes to table before quitting/overwriting and shows a warning
* Window title incorporates database filename
* CPack functionality in build system (macOS support beginning, but probably not yet functional)
* Program icon stored externally instead of internally (which was a huge waste of space)
* Added a .desktop file for Linux/BSD
* Added keyboard shortcuts
* Removed edit buttons, which overcomplicate the table handling and are largely unnecessary
* Toggling deleted entry visibility no longer repopulates the entire table, which should noticeably improve speed
* Added the foundations for localization with a Russian translation, though it will not install yet

# v0.1 - 05/14/2023
* Initial release