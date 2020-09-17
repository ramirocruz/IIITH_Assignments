# SSD Assignment 1
### There are some mentioned dependencies to run the file if required.
## Q1
* I had used the `rename` command since the task has to be performed in one command but since only local command had to be used I used `mv` inside `for` loop.
* But since looping seems to be out of question I have used `find` command with `mv`.
* I have commented the `rename` command and `loop` part also for reference.
* I have filled some data inside the text files to show the working of sort
* I have also created some new text files because the previous files are renamed
* I have assummed to also show  hidden folders in home directory showing all files.

## Q2
* I have first used the `compgen -c` command to list all commands and save them in `data.txt` in sorted order
* now i have sorted each command in ascending order and stored them in `commands.txt`
* I have created a `q2_helper.sh` file if you need the reference. If you want to run from scratch you can uncomment the first line in the `q2.sh` file.
* After first run please comment out the code to `amortize` the solution.
after this much `preprocessing` now when i recieve the input, i will first `sort` the it and then compare with the `sorted commands`.
* From there I will get the `line no.` and then use that `line no.` to `fetch` the actual command from `data.txt`
* I have also included both txt files 

## Q3
* First I took only the command field in the `.bash_history` file and `reversed it`.
* Now add them to my `commands list` if they are command and the total count is less than `10`.
* Now checking one by one if it is already checked then skip
* For every command I counted their occurences in the list of commands i created temporarily in a variable and then sorted by the count in reverse order.

## Q4
* I am using tr command to switch all `"(,)"` with `" "`.
* Then using awk to remove leading and trailing spaces.
* Later add one pair of paranthesis in the end.

## Q5
* used parameter expansion `{,,}` to convert all letters to `lowercase`
* used rev command to reverse the input string and stored it in other variable
compared if both are equal

## Q6
* I have used `bc` command to execute the exponent as the result might become large

## Q7
* I have pondered on this a lot. Running process at a time which are in **running state** will be 1 or 2 so its of no use.
* Then I thought of using only `aux` but that would be a lot of processes.
* Thats why I am using only `au`, its short and sweet and restricted to the terminal.
* I fetched all pid using `tr` and `cut` then `sorted` the pid and `stored` in file.
used `head` for first `n` pid

## Q8
* I have made a main function to check for the `validity` for each field.
* There are **two** `subsidiary functions` to be used for **validating the range** if given in the field or  the multiple values given by **commas**.
* One more function to check for the `@yearly` type schedules.

## Q9
* Trimmed the input then checked for any `illegal` character
* Again checked for the right `size` of the string
* Then traverse through the string backwards 
and calculated the respective sums

## Q10
* Used loop to store the operands in array.
* Used `tr` command to fill the gap with `operator`.
* Then used `bc` to calculate plus used `scale` field for precision.
* As the `bc` was truncating the `0` before decimal, I had to use `awk`.
* Used `sed` to remove any **trailing zeroes** from the answer.
