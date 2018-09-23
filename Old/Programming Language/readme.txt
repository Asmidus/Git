To run the program from your console:
1. Extract the folder
2. Navigate to the parser/debug folder, where parser.exe is located
3. Execute the "interpreter.exe" file with your desired arguments, input.txt file provided

If you wish to compile the .cpp file manually, you will need (on windows) the Developer Command Prompt for VS
or any other alternative c++ compiler you wish to use.
Using DCP for VS:
1. Open the DCP for VS
2. navigate to the parser/scanner folder, where parser.cpp is located
3. use this command: cl /W4 /EHsc file1.cpp file2.cpp file3.cpp /link /out:parser.exe
4. Execute "interpreter.exe" with your desired arguments, input.txt file provided

Example command:
scanner.exe input.txt output.txt

All text from input.txt will be tokenized and put into output.txt with the following format:

value, TOKEN TYPE

Followed by the requested format for navigating the tree structure

At the end, all variables will be shown in this format:

variable name: value