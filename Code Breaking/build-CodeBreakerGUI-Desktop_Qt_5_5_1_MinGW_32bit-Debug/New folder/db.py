import sqlite3, os
conn = sqlite3.connect('words.db')
c = conn.cursor()

c.execute("CREATE TABLE a(id int, word varchar(63))")
c.execute("CREATE TABLE b(id int, word varchar(63))")
c.execute("CREATE TABLE c(id int, word varchar(63))")
c.execute("CREATE TABLE d(id int, word varchar(63))")
c.execute("CREATE TABLE e(id int, word varchar(63))")
c.execute("CREATE TABLE f(id int, word varchar(63))")
c.execute("CREATE TABLE g(id int, word varchar(63))")
c.execute("CREATE TABLE h(id int, word varchar(63))")
c.execute("CREATE TABLE i(id int, word varchar(63))")
c.execute("CREATE TABLE j(id int, word varchar(63))")
c.execute("CREATE TABLE k(id int, word varchar(63))")
c.execute("CREATE TABLE l(id int, word varchar(63))")
c.execute("CREATE TABLE m(id int, word varchar(63))")
c.execute("CREATE TABLE n(id int, word varchar(63))")
c.execute("CREATE TABLE o(id int, word varchar(63))")
c.execute("CREATE TABLE p(id int, word varchar(63))")
c.execute("CREATE TABLE q(id int, word varchar(63))")
c.execute("CREATE TABLE r(id int, word varchar(63))")
c.execute("CREATE TABLE s(id int, word varchar(63))")
c.execute("CREATE TABLE t(id int, word varchar(63))")
c.execute("CREATE TABLE u(id int, word varchar(63))")
c.execute("CREATE TABLE v(id int, word varchar(63))")
c.execute("CREATE TABLE w(id int, word varchar(63))")
c.execute("CREATE TABLE x(id int, word varchar(63))")
c.execute("CREATE TABLE y(id int, word varchar(63))")
c.execute("CREATE TABLE z(id int, word varchar(63))")
c.execute("CREATE INDEX i_a ON a(word)")
c.execute("CREATE INDEX i_b ON b(word)")
c.execute("CREATE INDEX i_c ON c(word)")
c.execute("CREATE INDEX i_d ON d(word)")
c.execute("CREATE INDEX i_e ON e(word)")
c.execute("CREATE INDEX i_f ON f(word)")
c.execute("CREATE INDEX i_g ON g(word)")
c.execute("CREATE INDEX i_h ON h(word)")
c.execute("CREATE INDEX i_i ON i(word)")
c.execute("CREATE INDEX i_j ON j(word)")
c.execute("CREATE INDEX i_k ON k(word)")
c.execute("CREATE INDEX i_l ON l(word)")
c.execute("CREATE INDEX i_m ON m(word)")
c.execute("CREATE INDEX i_n ON n(word)")
c.execute("CREATE INDEX i_o ON o(word)")
c.execute("CREATE INDEX i_p ON p(word)")
c.execute("CREATE INDEX i_q ON q(word)")
c.execute("CREATE INDEX i_r ON r(word)")
c.execute("CREATE INDEX i_s ON s(word)")
c.execute("CREATE INDEX i_t ON t(word)")
c.execute("CREATE INDEX i_u ON u(word)")
c.execute("CREATE INDEX i_v ON v(word)")
c.execute("CREATE INDEX i_w ON w(word)")
c.execute("CREATE INDEX i_x ON x(word)")
c.execute("CREATE INDEX i_y ON y(word)")
c.execute("CREATE INDEX i_z ON z(word)")

indices = {}
filenames = []
for name in os.listdir():
	if '.' in name:
		if name.split('.')[1].isdigit():
			filenames.append(name)

small_words = []

for f in filenames:
	with open(f) as ifile:
		for line in ifile.readlines():
			line = line.upper()
			nline = ""
			for ch in line:
				if 64< ord(ch) < 91:
					nline += ch
			if len(nline) < 5:
				if len(nline) == 1 and not nline == "A" and not nline == "I":
					continue;
				small_words.append(nline)
			else:
				table = nline[0].lower()
				if not table in indices:
					indices[table] = 0
				c.execute("INSERT INTO {0} VALUES ({1},\'{2}\')".format(table,indices[table],nline))
				indices[table] += 1

# Save (commit) the changes
conn.commit()

# We can also close the connection if we are done with it.
# Just be sure any changes have been committed or they will be lost.
conn.close()

with open("small_words.txt", "w") as ofile:
	ofile.write("extern bool words_four[456976] = {")
	for c0 in ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]:
		for c1 in ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]:
			for c2 in ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]:
				for c3 in ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]:
					if c0+c1+c2+c3 in small_words:
						ofile.write("true,\n")
					else:
						ofile.write("false,\n")
	ofile.write("};\n")

	ofile.write("extern bool words_three[17576] = {")
	for c0 in ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]:
		for c1 in ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]:
			for c2 in ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]:
				if c0+c1+c2 in small_words:
					ofile.write("true,\n")
				else:
					ofile.write("false,\n")
	ofile.write("};\n")

	ofile.write("extern bool words_two[676] = {")
	for c0 in ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]:
		for c1 in ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]:
			if (c0+c1).lower() in ["ah", "am", "an", "as", "at", "be", "by", "do", "go", "ha", "he", "hi", "id", "if", "in", "is", "it", "me", "my", "no", "of", "oh", "on", "or", "ow", "so", "to", "up", "us", "we"]:
				ofile.write("true,\n")
			else:
				ofile.write("false,\n")
	ofile.write("};\n")

	ofile.write("extern bool words_one[26] = {")
	for c0 in ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]:
		if c0 in ["I","A"]:
			ofile.write("true,\n")
		else:
			ofile.write("false,\n")
	ofile.write("};\n")