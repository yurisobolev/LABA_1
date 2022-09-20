using System;
using System.IO;
using System.Text.Json;
using System.Threading.Tasks;
using System.Reflection;
using System.Text;
using System.Xml;
using System.Collections.Generic;
using System.IO.Compression;

namespace OS_Lab1_Sobolev_BBBO0221
{
    class User
    {
        public string Name { get; set; }
        public int Age { get; set; }
    }
    class Program
    {
        static string path = @"D:\Documents";
        public static async Task Main(string[] args)
        {
        mark:
            Console.WriteLine("Выберите номер задания от 1 до 5: ");
            string choice = Console.ReadLine();
            switch (choice)
            {
                case "1":
                    First();
                    goto mark;
                case "2":
                    Second();
                    goto mark;
                case "3":
                    await Third(args);
                    goto mark;
                case "4":
                    Fourth();
                    goto mark;
                case "5":
                    Fifth();
                    goto mark;
                default:
                    break;

            }
            //1 задание
            static void First()
            {
                DriveInfo[] drives = DriveInfo.GetDrives();

                foreach (DriveInfo drive in drives)
                {
                    Console.WriteLine($"Название: {drive.Name}");
                    if (drive.IsReady)
                    {
                        Console.WriteLine($"Объем диска: {drive.TotalSize}");
                        Console.WriteLine($"Свободное пространство: {drive.TotalFreeSpace}");
                        Console.WriteLine($"Метка диска: {drive.VolumeLabel}");
                        Console.WriteLine($"Тип диска: {drive.DriveType}");
                    }
                    Console.WriteLine();
                }
            }
            //2 задание
            static void Second()
            {


                DirectoryInfo dirInfo = new DirectoryInfo(path);
                if (!dirInfo.Exists)
                {
                    dirInfo.Create();
                }
                Console.WriteLine("Введите строку для записи в файл:");
                string text = Console.ReadLine();


                using (FileStream fstream = new FileStream($"{path}\\note.txt", FileMode.OpenOrCreate))
                {
                    byte[] array = System.Text.Encoding.Default.GetBytes(text);
                    fstream.Write(array, 0, array.Length);
                    Console.WriteLine("Текст записан в файл");
                }

                // чтение из файла
                using (FileStream fstream = File.OpenRead($"{path}\\note.txt"))
                {
                    byte[] array = new byte[fstream.Length];
                    fstream.Read(array, 0, array.Length);
                    string textFromFile = System.Text.Encoding.Default.GetString(array);
                    Console.WriteLine($"Текст из файла: {textFromFile}");
                    Console.ReadLine();
                }

                // удаление файла
                File.Delete($"{path}\\note.txt");
                Console.WriteLine("Файл удалён");
                Console.ReadLine();
            }
            //3 задание
            static async Task Third(string[] args)
            {

                // сохранение данных
                using (FileStream fs = new FileStream($"{path}\\user.json", FileMode.OpenOrCreate))
                {
                    User tom = new User() { Name = "Tom", Age = 35 };
                    await JsonSerializer.SerializeAsync<User>(fs, tom);
                    Console.WriteLine("Data has been saved to file");
                }

                // чтение данных
                using (FileStream fs = new FileStream($"{path}\\user.json", FileMode.OpenOrCreate))
                {
                    User restoredPerson = await JsonSerializer.DeserializeAsync<User>(fs);
                    Console.WriteLine($"Name: {restoredPerson.Name}  Age: {restoredPerson.Age}");
                    Console.ReadLine();
                }

                // удаление файла
                File.Delete($"{path}\\user.json");
                Console.WriteLine("Файл удалён");
                Console.ReadLine();
            }
            //4 задание

            static void Fourth()
            {

                List<User> users = new List<User>();
                XmlDocument xDoc = new XmlDocument();
                xDoc.Load($"{path}\\users.xml");
                XmlElement xRoot = xDoc.DocumentElement;

                // создаем новый элемент user
                XmlElement userElem = xDoc.CreateElement("user");
                XmlAttribute nameAttr = xDoc.CreateAttribute("name");
                XmlElement ageElem = xDoc.CreateElement("age");
                Console.WriteLine("Создание нового узла");
                Console.WriteLine("Введите имя пользователя: ");
                string name = Console.ReadLine();
                Console.WriteLine("Введите возраст пользователя: ");
                string age = Console.ReadLine();
                XmlText nameText = xDoc.CreateTextNode(name);
                XmlText ageText = xDoc.CreateTextNode(age);

                //добавляем узлы
                nameAttr.AppendChild(nameText);
                ageElem.AppendChild(ageText);
                userElem.Attributes.Append(nameAttr);
                userElem.AppendChild(ageElem);
                xRoot.AppendChild(userElem);
                xDoc.Save($"{path}\\users.xml");
                Console.WriteLine("Данные сохранены");
                //Вывод данных
                Console.WriteLine("Вывод данных");
                foreach (XmlElement xnode in xRoot)
                {
                    User user = new User();
                    XmlNode attr = xnode.Attributes.GetNamedItem("name");
                    if (attr != null)
                        user.Name = attr.Value;

                    foreach (XmlNode childnode in xnode.ChildNodes)
                    {
                        if (childnode.Name == "age")
                            user.Age = Int32.Parse(childnode.InnerText);
                    }
                    users.Add(user);
                }
                foreach (User u in users)
                    Console.WriteLine($"Имя: {u.Name} Возраст: {u.Age}");
                Console.ReadLine();

                // удаление файла
                File.Delete($"{path}\\users.xml");
                Console.WriteLine("Файл удалён");
                Console.ReadLine();
            }


            //5 задание
            static void Fifth()
            {
                string sourceFile = @"D:\Documents\user.txt";
                string compressedFile = @"D:\Documents\user.gz";
                string targetFile = @"D:\Documents\user_new.txt";

                Compress(sourceFile, compressedFile);

                Decompress(compressedFile, targetFile);
                Console.ReadLine();

                // удаление файла
                File.Delete(@"D:\Documents\user_new.txt");
                File.Delete(@"D:\Documents\user.gz");
                Console.WriteLine("Файлы удалены");
                Console.ReadLine();
            }

            static void Compress(string sourceFile, string compressedFile)
            {
                using (FileStream sourceStream = new FileStream(sourceFile, FileMode.OpenOrCreate))
                {
                    using (FileStream targetStream = File.Create(compressedFile))
                    {
                        using (GZipStream compressionStream = new GZipStream(targetStream, CompressionMode.Compress))
                        {
                            sourceStream.CopyTo(compressionStream);
                            Console.WriteLine("Сжатие файла {0} завершено. Исходный размер: {1}  сжатый размер: {2}.",
                                sourceFile, sourceStream.Length.ToString(), targetStream.Length.ToString());
                        }
                    }
                }
            }
            ///Разархивирование
            static void Decompress(string compressedFile, string targetFile)
            {
                using (FileStream sourceStream = new FileStream(compressedFile, FileMode.OpenOrCreate))
                {
                    using (FileStream targetStream = File.Create(targetFile))
                    {
                        using (GZipStream decompressionStream = new GZipStream(sourceStream, CompressionMode.Decompress))
                        {
                            decompressionStream.CopyTo(targetStream);
                            Console.WriteLine("Восстановлен файл: {0}", targetFile);
                        }
                    }
                }
            }
        }
    }

}

/*Перед выполнением задания 4 создать файл вида:
<?xml version="1.0" encoding="utf-8"?>
<users>
</users> 
*/
