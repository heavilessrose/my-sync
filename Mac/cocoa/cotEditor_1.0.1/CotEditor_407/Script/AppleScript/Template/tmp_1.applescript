(*
�őO�ʂ̃E�B���h�E�̑I��͈͂� property newStr  �ɐݒ肳�ꂽ������Œu��������X�N���v�g�B
���̗�ł́A�uTEMPLATE�v�ɒu�����܂��B������́A�uTEMPLATE�v��I����Ԃɂ��܂��B
*)
(* written by nakamuxu. 2005.04.14 *)
--
property newStr : "TEMPLATE"
--
--
tell application "CotEditor"
	if exists front document then
		set contents of selection of front document to newStr
	end if
end tell