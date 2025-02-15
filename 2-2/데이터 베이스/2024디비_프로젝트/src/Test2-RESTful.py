from flask import Flask, request, jsonify
import pymysql

app = Flask(__name__)
id = 70216008
db = pymysql.connect(
    host="119.3.248.160",
    user=f"{id}",
    password=f"{id}",
    database=f"db_{id}",
    port=3306
)


@app.route('/api/v1/<table_name>', methods=['POST'])
def insert_data(table_name):
    data = request.json
    rows = data['rows']
    cursor = db.cursor()
    try:
        count = 0
        for row in rows:
            cursor.execute(f"SELECT MAX(id) FROM {table_name}")
            max_id = cursor.fetchone()[0]
            new_id = max_id + 1 if max_id else 1
            

            row['id'] = new_id
            
            keys = ', '.join(row.keys())
            values = ', '.join([f"'{value}'" for value in row.values()])
            sql = f'INSERT INTO {table_name}({keys}) VALUES({values})'
            cursor.execute(sql)
            count += 1  
        
        db.commit()
        return jsonify({'message': f"{count} rows inserted into {table_name} successfully"}), 201  
    except Exception as e:
        db.rollback()
        return jsonify({'error': str(e)}), 500
    finally:
        cursor.close() 


# 更新数据
@app.route('/api/v1/<table_name>', methods=['PUT'])
def update_data(table_name):
    data = request.json
    rows = data['rows']
    cursor = db.cursor()
    for row in rows:
        primary_key_name = 'id'
        primary_key_value = row[primary_key_name]
        set_clause = ', '.join([f"{key}='{value}'" for key, value in row.items() if key != primary_key_name])
        
        sql = f"UPDATE {table_name} SET {set_clause} WHERE {primary_key_name} = '{primary_key_value}'"
        cursor.execute(sql)
    db.commit()
    return jsonify({'message': 'data updated successfully'}), 200

# TODO: finish this fuction to update data


# 删除数据
@app.route('/api/v1/<table_name>/<path:args>', methods=['DELETE'])
def delete_data(table_name, args):
    cursor = db.cursor()
    sql = f"DELETE FROM {table_name} WHERE id = {id}"
    cursor.execute(sql)
    db.commit()
    return jsonify({'message': 'data deleted successfully'}), 200

# TODO: finish this fuction to update data


@app.route('/api/v1/<table_name>/<path:args>', methods=['GET'])
def select_data(table_name, args):
    cursor = db.cursor()
    cursor.execute(f"SELECT * FROM {table_name} WHERE id = {id}")
    result = cursor.fetchall()
    return jsonify(result), 200

# TODO: finish this fuction to update data


@app.route('/api/v1/<table_name>', methods=['GET'])
def select_data_condition(table_name):
    query_params = request.args
    condition = ' AND '.join([f"{key}='{value}'" for key, value in query_params.items()])
    sql = f"SELECT * FROM {table_name} WHERE {condition}"
    cursor = db.cursor()
    cursor.execute(sql)
    result = cursor.fetchall()
    return jsonify(result), 200

# TODO: finish this fuction to update data


if __name__ == '__main__':
    app.run(debug=False, port=5555, host='0.0.0.0')
