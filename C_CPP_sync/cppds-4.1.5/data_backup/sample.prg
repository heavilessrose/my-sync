:$b = 10
:$c = 0
:$n = 5
:for($a,0,$n,1)
:disp "a^2=",$a^2
:if $a > 2
:then
:disp "a > 2"
:else
:disp "a <= 2"
:end
:is>($c,3)
:disp "c <= 3"
:disp "c=",$c
:ds<($b,7)
:disp "b >= 7"
:disp "b=",$b
:prgm sample2
:end
