# terminal
Simple library to manage terminal output and input.

## Output
Creates terminal object.  This object is used like a stream object.
For Example:

    terminal << save << Location(10,10) << "Hello" << restore;

Will save the location of the cursor, move the cursor to (10,10) print "Hello" and then return the cursor to its previous location.

   terminal << clear;

Will clear the terminal


## Input

Allows the user to create an object to handle input.
    
    InputHandler<Prompt> inputHandler(terminal, Prompt("Enter Some text: ", Location(0,2)), inputFunc); 

Will move the cursor to the location (0,2) and display the text "Enter Some text:".  When the user hits enter, it will call inputFunc type (`std::function<bool(std::string)>`).  This function gets the string entered and should return true if you want to display the same prompt again.

