# Security system
Incomplete security system, that uses OpenCV DNN for face detection and LPBH for recognition. GTK+ gtkmm GUI, MySQL database, Nvidia CUDA.

# Compile
First you need to create some folders and add some files.
1. Create "build" folder.
2. Create "output" folder.
3. Create "att_faces" folder in "assets".
4. Create "0" folder in "att_faces".
5. Add in "0" folder 7-9 photos of the person you want to recognize.

Change working folder to "build" and enter:
   ```sh
   $ cmake ..
   $ make
   ```
The program is compiled, but so far it does not work.

# Run
Now you need to setup database.
1. Create database with name "company_staff" or something. You can change this in code.
2. Create table with name "employees" or something. You can also change this in code.
3. Create column like this:
```csv
+-------------+-------------+------+-----+---------+----------------+
| Field       | Type        | Null | Key | Default | Extra          |
+-------------+-------------+------+-----+---------+----------------+
| id          | int(11)     | NO   | PRI | NULL    | auto_increment |
| name        | varchar(64) | NO   |     | NULL    |                |
| position    | varchar(64) | YES  |     | NULL    |                |
| birth_date  | date        | NO   |     | NULL    |                |
| hiring_date | date        | NO   |     | NULL    |                |
| database_id | int(11)     | NO   | UNI | NULL    |                |
+-------------+-------------+------+-----+---------+----------------+
```
4. Fill rows. "database_id" should be the same as the photo folder.
5. Change "server", "user" and "password" strings in "sql_con.cpp" file.

Now you should recompile the program.
Run security_system file!
