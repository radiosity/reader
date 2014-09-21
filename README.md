#reader

This unemaginatively titled piece of software is a Linux e-book reader written in GTK+. 

Its dependence on GTK means that it's most at homes on GNOME (or GNOME-based) distributions, but should work fine on others. Furthermore, most I/O is abstracted by boost which should make it possible to port to other operating systems in the future if required. 

The reader supports a subset of the CSS commonly found within EPUB based ebooks. It is extremely unlikely that the full functionality of CSS will be implemented as this would be too great a restriction on the ability of the reader to re-flow the text to make it attractive. 

Books are imported into a library through the use of SQLite. 

