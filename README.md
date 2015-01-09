# terminal
Simple library to manage terminal output.

Creates terminal object.  This object is used like a stream object.
For Example:

terminal << save << Location(10,10) << "Hello" << restore;

Will save the location of the cursor, move the cursor to (10,10) print "Hello" and then return the cursor to its previous location.

