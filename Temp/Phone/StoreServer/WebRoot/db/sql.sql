; stores.db

CREATE TABLE [tbl_Stores] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[name] TEXT  UNIQUE NOT NULL,
[position] TEXT  NULL,
[dbName] TEXT  NULL
);


;[store_map].db
CREATE TABLE [tbl_Map] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[storeId] INTEGER  NULL,
[floor] INTEGER  UNIQUE NOT NULL,
[map] BLOB  NULL,
[path] BLOB  NULL
);

;create unique index on storeId + floor
CREATE UNIQUE INDEX [idx_tbl_Map_Store_Floor] ON [tbl_Map](
[storeId]  DESC,
[floor]  DESC
);

CREATE TABLE [tbl_Position] (
[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[floorId] INTEGER  NOT NULL,
[PosX] INTEGER  NOT NULL,
[PosY] INTEGER  NOT NULL,
[Desc] TEXT  NULL
)
CREATE INDEX [idx_tbl_Position_FloorId] ON [tbl_Position](
[floorId]  DESC
)

