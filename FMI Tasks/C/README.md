**2   System calls (functions provided by the kernel)<br>3   Library calls (functions within program libraries)**

attribute|Shell Scriopts|C
:---|:---:|:---:
abstraction level|high level|low level
man pages|1|2,3|
type system|no|yes (char, uint8_t, struct)
'#' of arguments|$#|argc
arguments|$1, $2|argv[1], argv[2], ...
compiled?|no|yes (make)
memory management|no|pointers
performance|slow|fast
