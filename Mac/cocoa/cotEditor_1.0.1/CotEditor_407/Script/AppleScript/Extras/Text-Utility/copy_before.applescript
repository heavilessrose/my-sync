(*
�őO�ʂ̃E�B���h�E�̑I��͈͂̒���ɁA�I��͈͂��R�s�[����X�N���v�g�B
Mac OS X 10.5+ �őI��͈͂̒��O�^����ւ̃h���b�O&�h���b�v���ł��Ȃ��Ȃ������Ƃւ̑΍�Ƃ��č쐬�B
*)
(* written by nakamuxu. 2008.01.05 *)
--
--
--
tell application "CotEditor"
	if exists front document then
		set oldRange to range of selection as list
		set len to item 2 of oldRange
		if len > 0 then
			set copyStr to contents of selection of front document
			set loc to item 1 of oldRange
			set range of selection of front document to {loc, 0}
			set contents of selection of front document to copyStr
		end if
	end if
end tell