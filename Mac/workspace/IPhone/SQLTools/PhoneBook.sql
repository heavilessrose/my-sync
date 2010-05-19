CREATE TABLE IF NOT EXISTS main.phonebook (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, ContactID INTEGER UNIQUE NOT NULL DEFAULT 1, ShowName TEXT NOT NULL DEFAULT NoName, MainPhone TEXT, MobilePhone TEXT, HomePhone TEXT, HomeFax TEXT, WorkPhone TEXT, WorkFax TEXT, OtherPhone TEXT, Pager TEXT, Email TEXT, Avatar BLOB);


INSERT INTO PhoneBook_PICA (ContactID, MainPhone, Email) VALUES (0100, '15965986632', 'test@test.com');
INSERT INTO PhoneBook_PICA (ContactID, MainPhone, Email) VALUES (0101, '15965986888', 'test2@test.com');
