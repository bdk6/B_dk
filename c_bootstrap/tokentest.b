svar ;
string "This is a string";

main()
{
  auto ident;
  auto mystring "This is a string";
  if(ident < 99)
  {
    ident = ident + 4;
    ident =+ 1;
    ident =* 3;
    ident =/ 2;
    ident =- 1;
    ident =% 9;
    ident =| 8;
    ident =& 9;
    ident === 3;
    ident =!= 2;
  }
  ident = ident + 4 * 3 / 5;
  return ident;
}
