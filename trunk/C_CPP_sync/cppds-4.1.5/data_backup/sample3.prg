:input "betting on dice value = ",$g
:input "max rounds = ",$h
:menu("toss dice","weighted dice",A,"fair dice",B)
:lbl A
:prompt $a,$b,$c,$d,$e,$f
:lbl C
:for($i,0,$h,1)
:  $j = $a + $b + $c + $d + $e + $f
:  $k = RAND($j)
:  if ($k >= 0) & ($k < $a)
:  then
:    disp "1 is rolled"
:    if $g == 1
:    then
:      pause "you win"
:    end
:  else
:    if ($k >= $a) & ($k < $a + $b)
:    then
:      disp "2 is rolled"
:      if $g == 2
:      then
:        pause "you win"
:      end
:    else
:      if ($k >= $a + $b) & ($k < $a + $b + $c)
:      then
:        disp "3 is rolled"
:        if $g == 3
:        then
:          pause "you win"
:        end
:      else
:        if ($k >= $a + $b + $c) & ($k < $a + $b + $c + $d)
:        then
:          disp "4 is rolled"
:          if $g == 4
:          then
:            pause "you win"
:          end
:        else
:          if ($k >= $a + $b + $c + $d) & ($k < $a + $b + $c + $d + $e)
:          then
:            disp "5 is rolled"
:            if $g == 5
:            then
:              pause "you win"
:            end
:          else
:            if ($k >= $a + $b + $c + $d + $e) & ($k < $a + $b + $c + $d + $e + $f)
:            then
:              disp "6 is rolled"
:              if $g == 6
:              then
:                pause "you win"
:              end
:            end
:          end
:        end
:      end
:    end
:  end
:end
:goto End
:lbl B
:$a = $b = $c = $d = $e = $f = 1
:goto C
:lbl End
:pause "game over"
