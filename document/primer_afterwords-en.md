# Afterwords
Error and Warning Codes and Known Inconveniences.

## Compile options

|option|comment|
|---|---|
|-o filename|specify output filename||
|-v|Verbose mode|

## Error code
* parse error
 * syntax error
* syntax error
 * string error

## Warning code

### Boundary of variable area has shifted.
This message is displayed when the address assignment of a variable becomes an address that is not divisible.
For example, if you put a float type behind a char type variable, 3 bytes of unused space will be allocated between the char and the float.

## Known Inconveniences
Not completed, will be summarized at a later date.

## Postscript
Please wait a while.

## References
John R. Levine and Tony Mason and Doug Brown, lex & yacc.
Donnely and Stallman, The Bison Manual. Part of the Bison Distribution Kit
Kenji Gogetsu, yacc/lex program generator on UNIX
Ken-ichi Harada, Compiler Configuration