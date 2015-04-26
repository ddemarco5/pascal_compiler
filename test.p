program Hello(input, output);
var x,y,z : integer;
var H : integer;
var q : array[1 .. 2] of real;

(*
procedure boo;
begin
  boo := x
end;
*)

function foo(a: integer): integer;
begin
(*  H := H + 0;
  if x = 0 then x := 1
  else x := 2;
  while x = 1 do x := x + x;
  a := H; *)
  write(1);
  foo := 1
end;

function boo(c:real): integer;
begin
  boo := 1
end;


begin
  //H := foo(foo(foo(1)));
  foo(foo(1))
  (*x := (5+1)*3
  x[1+1] := x[2+x]
  q[1] := 1;
  for x := 0 to 10 do x := x + 1*)
end.
