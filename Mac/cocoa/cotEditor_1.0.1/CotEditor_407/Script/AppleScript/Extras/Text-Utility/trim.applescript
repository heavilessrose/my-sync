(*
�őO�ʂ̃E�B���h�E�̍s���^�s���̋󔒂��폜����X�N���v�g�B
*)
(* written by nakamuxu. 2008.01.05 *)
--
--
--
tell application "CotEditor"
	if exists front document then
		replace front document for "^ +" to "" with RE and all
		replace front document for " +$" to "" with RE and all
	end if
end tell