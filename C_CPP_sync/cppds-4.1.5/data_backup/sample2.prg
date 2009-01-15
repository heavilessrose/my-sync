:$d = 5
:$e = 3
:while ($d^2 > 10)
:disp "d^2 > 10, ",$d^2
:if $d @ 2
:then
:goto odd_d
:end
:disp "d is even, ",$d
:goto skipodd_d
:lbl odd_d
:disp "d is odd, ",$d
:lbl skipodd_d
:$d -= 1
:end
:disp "d^2 < 10, ",$d^2

:repeat ($e^2 == 25)
:disp "e^2 != 25, ",$e^2
:if $e @ 2
:then
:goto odd_e
:end
:disp "e is even, ",$e
:goto skipodd_e
:lbl odd_e
:disp "e is odd, ",$e
:lbl skipodd_e
:$e += 1
:end
:disp "e^2 == 25, ",$e^2
