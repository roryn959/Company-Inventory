Company Stock Database

This application allows a company to store details about the items it has in stock.

To run:
1. Navigate to file location in directory in terminal
2. Compile code using command 'gcc -o logger src.c'
3. Run code using command './logger.exe'

A user must enter login details in order to view and edit the stock. Initially, there will be no login details to use - you must create an account as 
prompted by the program by typing '0'.

In order to create an account, the user will be asked for an admin key. This is provided in the file 'key.txt'. This is to prevent an unauthorised user from
adding their own login.

This will create a file called 'logins.txt' containing the login details of users (ideally the details would be encrypted).

If details are correctly entered, a menu will be shown, asking for a selection. Type '0' for a help menu explaining the options to choose from. Using this 
you can edit and update the stock of the company. The data will be stored in a text file called 'data.txt'.

To exit program, log out using option 7 when prompted, and then confirm by typing 'Y'. After logging out, type 'E' to terminate the program.

