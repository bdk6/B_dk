svar ;
nuthervar;

string "This is a string";

main(argc, argv)
{
  auto ident;
  auto mystring "This is a string";
  extrn extvar;
  extrn svar;
  svar = svar * argc + 2;
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
    ident = main();
  }
  else ident = 0;
  ident = ident + 4 * 3 / 5;
  return ident;
}

fn2()
{
  auto x;
  extrn main;
  extrn string;

  while(1)
  {
    if(x>5)
      x = x*2;
  }
}

