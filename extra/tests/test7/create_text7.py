"""
По причине, что на GitHub нельзя загрузить большие по размеру файлы,
то будет показана программа, которая создает этот файл
"""


# Чтение из файла
with open('text1.txt', 'r', encoding='utf-8') as file:
    contents = file.read()

# Умножаем содержимое на 1215
result = contents * 1215

# Запись в новый файл
with open('text7.txt', 'w', encoding='utf-8') as file:
    file.write(result)

