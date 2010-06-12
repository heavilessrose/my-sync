CREATE TABLE IF NOT EXISTS phonebook (\
	PK INTEGER PRIMARY KEY AUTOINCREMENT, \
	phonebook_id TEXT UNIQUE NOT NULL DEFAULT '0', \
	disp_name TEXT NOT NULL DEFAULT 'NoName', \
	main_phone TEXT, \
	mobile_phone TEXT, \
	home_phone TEXT, \
	home_fax TEXT, \
	work_phone TEXT, \
	work_fax TEXT, \
	other_phone TEXT, \
	pager TEXT, \
	email TEXT, \
	portrait TEXT)


INSERT INTO PhoneBook_PICA (ContactID, MainPhone, Email) VALUES (0100, '15965986632', 'test@test.com');
INSERT INTO PhoneBook_PICA (ContactID, MainPhone, Email) VALUES (0101, '15965986888', 'test2@test.com');


CREATE TABLE IF NOT EXISTS user_association (\
	UID INTEGER PRIMARY KEY AUTOINCREMENT, \
	phonebook_id TEXT UNIQUE, \
	facebook_id TEXT UNIQUE, \
	msn_imid TEXT UNIQUE)

CREATE TABLE IF NOT EXISTS group_association (\
	PK INTEGER PRIMARY KEY AUTOINCREMENT, \
	group_id TEXT NOT NULL DEFAULT 0, \
	UID INTEGER)

CREATE TABLE IF NOT EXISTS user_group(\
	GID INTEGER PRIMARY KEY AUTOINCREMENT, \
	group_name TEXT UNIQUE NOT NULL, \
	group_passwd TEXT)