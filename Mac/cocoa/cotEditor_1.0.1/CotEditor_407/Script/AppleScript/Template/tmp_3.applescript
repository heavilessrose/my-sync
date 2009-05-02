(*
�őO�ʂ̃E�B���h�E�̑I��͈͂� property beginStr �� endStr  �ɐݒ肳�ꂽ������ň͂ރX�N���v�g�B
���̗�ł́A�u<h1>�v�Ɓu</h1>�v�ň݂͂܂��B������́A�I��͈͂��������ꍇ�ɂ́u</h1>�v�̒���ɁA�Ȃ������ꍇ�͑O�ɁA�L�����b�g���ړ����܂��B
property preMargin ��0 �ȊO�̐��l������ƁA�����O�̑I��͈͂̊J�n�ʒu���� preMargin �������A�L�����b�g�������I�Ɉړ��ł��܂��B
*)
(* written by nakamuxu. 2005.04.14 *)
--
property beginStr : "<h1>"
property endStr : "</h1>"
property preMargin : 0
--
--
tell application "CotEditor"
	if exists front document then
		set {loc, len} to range of selection of front document
		if (len = 0) then
			set newStr to beginStr & endStr
			if (preMargin = 0) then
				set numOfMove to count of character of beginStr
			else
				set numOfMove to preMargin
			end if
		else if (len > 0) then
			set curStr to contents of selection of front document
			set newStr to beginStr & curStr & endStr
			if (preMargin = 0) then
				set numOfMove to count of character of newStr
			else
				set numOfMove to preMargin
			end if
		else
			return
		end if
		set contents of selection of front document to newStr
		set range of selection of front document to {loc + numOfMove, 0}
	end if
end tell