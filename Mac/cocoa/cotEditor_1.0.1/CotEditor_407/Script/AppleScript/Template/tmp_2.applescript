(*
�őO�ʂ̃E�B���h�E�̑I��͈͂� property newStr  �ɐݒ肳�ꂽ������Œu��������X�N���v�g�B
���̗�ł́A�uTEMPLATE�v�ɒu�����܂��B������́A�uTEMPLATE�v�̒���ɃL�����b�g���ړ����܂��B
*)
(* written by nakamuxu. 2005.04.14 *)
--
property newStr : "TEMPLATE"
--
--
tell application "CotEditor"
	if exists front document then
		set {loc, len} to range of selection of front document
		set numOfMove to count of character of newStr
		set contents of selection of front document to newStr
		set range of selection of front document to {loc + numOfMove, 0}
	end if
end tell