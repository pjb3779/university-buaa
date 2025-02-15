from flask import Flask, jsonify, request
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import event, create_engine, text
from datetime import datetime
from sqlalchemy.orm import sessionmaker
import csv
import time

id = 70216008
app = Flask(__name__)
app.config[
    'SQLALCHEMY_DATABASE_URI'] = f"mysql+pymysql://{id}:{id}@119.3.248.160:3306/db_{id}?charset=utf8mb4"
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)
engine = create_engine(f"mysql+pymysql://{id}:{id}@119.3.248.160:3306/db_{id}?charset=utf8mb4")
# print(db)
Session = sessionmaker(bind=engine)
session = Session()


class Departments(db.Model):
    dept_no = db.Column(db.CHAR(4), primary_key=True)
    dept_name = db.Column(db.String(40), nullable=False, unique=True)
    dept_emps = db.relationship('Dept_emp', backref='department', lazy=True, cascade='all, delete')
    dept_managers = db.relationship('Dept_manager', backref='department', lazy=True, cascade='all, delete')

    def to_dict(self):
        return {
            'dept_no': self.dept_no,
            'dept_name': self.dept_name
        }


class Employees(db.Model):
    emp_no = db.Column(db.Integer, primary_key=True)
    birth_date = db.Column(db.Date, nullable=False)
    first_name = db.Column(db.String(14), nullable=False)
    last_name = db.Column(db.String(16), nullable=False)
    gender = db.Column(db.Enum('M', 'F'), nullable=False)
    hire_date = db.Column(db.Date, nullable=False)

    def to_dict(self):
        return {
            'emp_no': self.emp_no,
            'birth_date': self.birth_date,
            'first_name': self.first_name,
            'last_name': self.last_name,
            'gender': self.gender,
            'hire_date': self.hire_date
        }

# TODO: Finish the Employees Class to create the table

class Dept_emp(db.Model):
    emp_no = db.Column(db.Integer, db.ForeignKey('employees.emp_no'), primary_key = True )
    dept_no = db.Column(db.CHAR(4), db.ForeignKey('departments.dept_no'),primary_key = True )
    from_date = db.Column(db.Date, nullable = False)
    to_date = db.Column(db.Date, nullable = False)

    def to_dict(self):
        return{
            'emp_no': self.emp_no,
            'dept_no': self.dept_no,
            'from_date': self.from_date,
            'to_date': self.to_date
        }
# TODO: Finish the Dept_emp Class to create the table

class Dept_manager(db.Model):
    emp_no = db.Column(db.Integer, db.ForeignKey('employees.emp_no'), primary_key = True )
    dept_no = db.Column(db.CHAR(4), db.ForeignKey('departments.dept_no'),primary_key = True )
    from_date = db.Column(db.Date, nullable = False)
    to_date = db.Column(db.Date, nullable = False)

    def to_dict(self):
        return{
            'emp_no': self.emp_no,
            'dept_no': self.dept_no,
            'from_date': self.from_date,
            'to_date': self.to_date
        }

# TODO: Finish the Dept_manager Class to create the table

class Titles(db.Model):
    emp_no = db.Column(db.Integer, db.ForeignKey('employees.emp_no'), primary_key=True)
    title = db.Column(db.String(50), primary_key=True)
    from_date = db.Column(db.Date, primary_key=True)
    to_date = db.Column(db.Date)

    def to_dict(self):
        return {
            'emp_no': self.emp_no,
            'title': self.title,
            'from_date': self.from_date,
            'to_date': self.to_date
        }

# TODO: Finish the Titles Class to create the table

class Dept_manager_title(db.Model):
    emp_no = db.Column(db.Integer, primary_key=True)
    from_date = db.Column(db.Date, nullable=False)
    to_date = db.Column(db.Date, nullable=True)

    def to_dict(self):
        return {
            'emp_no': self.emp_no,
            'from_date': self.from_date,
            'to_date': self.to_date
        }

# TODO: Finish the Dept_manager_title Class to create the table


sql_trigger_insert = text("""
CREATE TRIGGER AfterInsertDeptManager
AFTER INSERT ON dept_manager
FOR EACH ROW
BEGIN
    INSERT INTO dept_manager_title (emp_no, from_date, to_date)
    VALUES (NEW.emp_no, NEW.from_date, NEW.to_date);
END;
""")

sql_trigger_delete = text("""
CREATE TRIGGER AfterDeleteDeptManager
AFTER DELETE ON dept_manager
FOR EACH ROW
BEGIN
    DELETE FROM dept_manager_title WHERE emp_no = OLD.emp_no;
END;
""")

sql_index_employees_first_name = text("""
CREATE INDEX idx_first_name ON employees (first_name);
""")

sql_index_dept_emp_dept_no = text("""
CREATE INDEX idx_dept_emp_dept_no ON dept_emp (dept_no);
""")

sql_index_dept_manager_dept_no = text("""
CREATE INDEX idx_dept_manager_dept_no ON dept_manager (dept_no);
""")

# TODO: Finish the insert and delete trigger by yourself

def read_csv_file(file_path):
    rows = []
    with open(file_path, 'r', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            rows.append(row)
    return rows


with app.app_context():
    db.drop_all()
    db.create_all()

with engine.connect() as connect:
    connect.execute(sql_trigger_insert)
    connect.execute(sql_trigger_delete)
    connect.execute(sql_index_employees_first_name)
    connect.execute(sql_index_dept_emp_dept_no)
    connect.execute(sql_index_dept_manager_dept_no)

# department插入
reader = read_csv_file('C:\\Users\\버미\\Desktop\\대학 2-2\\데이터베이스\\2024数据管理技术期中大作业\\2024数据管理技术期中大作业\\数据管理技术期中大作业材料\\departments.csv')

# data = [{'dept_no': row['dept_no'], 'dept_name': row['dept_name']} for row in reader]
rows = []
for row in reader:
    department = Departments(
        dept_no=row['dept_no'],
        dept_name=row['dept_name']
    )
    rows.append(department)

session.bulk_save_objects(rows)
session.commit()

# employees插入
# TODO: insert data
reader = read_csv_file(r'C:\Users\버미\Desktop\대학 2-2\데이터베이스\2024数据管理技术期中大作业\2024数据管理技术期中大作业\数据管理技术期中大作业材料\employees.csv')
rows = []
for row in reader:
    employee = Employees(
        emp_no=int(row['emp_no']),
        birth_date=datetime.strptime(row['birth_date'], '%Y-%m-%d').date(),
        first_name=row['first_name'],
        last_name=row['last_name'],
        gender=row['gender'],
        hire_date=datetime.strptime(row['hire_date'], '%Y-%m-%d').date()
    )
    rows.append(employee)

session.bulk_save_objects(rows)
session.commit()

# dept_emp插入
# TODO: insert data
reader = read_csv_file(r'C:\Users\버미\Desktop\대학 2-2\데이터베이스\2024数据管理技术期中大作业\2024数据管理技术期中大作业\数据管理技术期中大作业材料\dept_emp.csv')
rows = []
for row in reader:
    dept_emp_record = Dept_emp(
        emp_no=int(row['emp_no']),
        dept_no=row['dept_no'],
        from_date=datetime.strptime(row['from_date'],'%Y-%m-%d').date(),
        to_date=datetime.strptime(row['to_date'], '%Y-%m-%d').date()
    )
    rows.append(dept_emp_record)

session.bulk_save_objects(rows)
session.commit()

# dept_manager插入
# TODO: insert data
reader = read_csv_file(r'C:\Users\버미\Desktop\대학 2-2\데이터베이스\2024数据管理技术期中大作业\2024数据管理技术期中大作业\数据管理技术期中大作业材料\dept_manager.csv')
rows =[]
for row in reader:
    dept_manager_record = Dept_manager(
        emp_no=int(row['emp_no']),
        dept_no=row['dept_no'],
        from_date=datetime.strptime(row['from_date'], '%Y-%m-%d').date(),
        to_date=datetime.strptime(row['to_date'], '%Y-%m-%d').date()
    )
    rows.append(dept_manager_record)

session.bulk_save_objects(rows)
session.commit()
# titles插入
# TODO: insert data
reader = read_csv_file(r'C:\Users\버미\Desktop\대학 2-2\데이터베이스\2024数据管理技术期中大作业\2024数据管理技术期中大作业\数据管理技术期中大作业材料\titles.csv')
rows = []
for row in reader:
    title_record = Titles(
        emp_no=int(row['emp_no']),
        title=row['title'],
        from_date=datetime.strptime(row['from_date'], '%Y-%m-%d').date(),
        to_date=datetime.strptime(row['to_date'], '%Y-%m-%d').date() if row['to_date'] else None
    )
    rows.append(title_record)

session.bulk_save_objects(rows)
session.commit()