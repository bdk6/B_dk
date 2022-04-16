main()
{
  auto ident;
  if(ident < 99)
  {
    ident =+ 1;
    ident++;
    ident =* 3;
    ident =/ 2;
    ident =- 1;
    ident =% 9;
    ident =| 8;
    ident =& 9;
    ident === 3;
    ident =!= 2;
  }
  return ident;
}
