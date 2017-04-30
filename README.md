# Shell_Script_C
Shell Script implementation in c

It's s Mini-Project Given As a Adavnced Operating System course in BITS GOA

Breif Description
- Program starts with 'Begin' word and ends with 'End' word.
- All other statements are considered as a comment (Ignore them)
- One line comment is start with '#'
- Multiple line comment starts with /* ...... */
- ALL output of each instruction is written in 'OUTPUT.txt' file
- If anywhere it starts with 'INTERSTART' ....every instruction output is written into separate file.

Execution of Normal instruction is easy task...

To execute instruction which are in pipeline ---- required some mechanisam to implement that

Solution 
- Take two file 'temp1' and 'temp2' 
- Use both file interchangably to to read/write(stdin/stdout) output of previous instruction...
- This is most effective solution for execution instructions which are in pipeline..
