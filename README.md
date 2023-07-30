# SeaBattle
_Игра "Морской Бой", реализованная на языке С++ с применением ООП (консольное приложение)._

---

### Содержание
[Класс _ship_](#ship) \
[Класс _board_](#board)\
[Класс _Player_](#player)\
[Класс _Human_](#human)\
[Класс _Bot_](#bot)\
[Класс _SeaBattle_](#seab)\
[Класс _Console_](#cons)\
[Обзор игры](#todo)
## Краткое описание разработки
Разработка начиналась с создания пустого проекта в Visual Studio 2019 и написания кода с нуля. 
Перед этим я смотрела другие проекты, но у них была другая реализация, поэтому я взяла только советы к ним по написанию кода.\
У меня было несколько идей, как реализовать алгоритм игры, но я остановилась на одной: проще и лучше было
разработать алгоритм игры, используя ООП. Ниже приведена таблица с классами:
|Название класса|Базовый класс|Описание класса|
|:-|:-|:-|
|ship|Нет|Корабль для игры|
|board|Нет|Игровое поле (10x10)|
|Player|Нет|Обобщенное описание игрока<br />_Абстрактный класс_|
|Human|Player|Человек − игрок|
|Bot|Player|Бот − игрок|
|SeaBattle|Нет|Игра "Морской бой"|
|Console|Нет|Дополнительные функции<br />для работы программы|

<a name="ship"><h3>Класс _ship_</h3></a>
Класс описывает корабль. В процессе игры `m_life` будет уменьшаться.
|Поле класса _ship_|Тип|Описание поля|
|:-|:------|:------|
|m_deck|int|Количество палуб|
|m_life|int|Количество жизней|

<a name="board"><h3>Класс _board_</h3></a>
Класс представляет собой игровое поле размером 10х10.\
Первое поле − массив указателей на объекты `ship`. Именно по этому полю определяем: попадание/непопадание, 
а также сразу идентифицируем, в какой корабль попали и сколько осталось попаданий до уничтожения (для корректной работы алгоритма; ни человек, ни бот не знают изначально эти данные).\
Второе поле − массив символов. Именно его будет видеть игрок и взаимодействовать с ним. 
|Поле класса _board_|Тип|Описание поля|
|:-|:------|:------|
|p_board[ ]|ship*|Поле ссылок на корабли|
|m_board[ ]|char|Основное поле|
|m_steps[ ]|int|Порядок кораблей|
|m_step|int|Номер действия|

Третьего и четвертого полей изначально не было. Они появились для того, чтобы отменить последний поставленный корабль: 
после того, как игрок поставил корабль, `m_step` увеличивается на единицу, и это значение ставится в `m_steps[]` там, где ставится символ на игровом поле `m_board[]`. 
Если действие отменить, то удаляются только те символы на игровом поле `m_board[]`, где `m_steps[]` равно `m_step`. Затем `m_step` уменьшается на единицу.
<a name="player"><h3>Класс _Player_</h3></a>
Класс описывает абстрактного игрока. У него есть чисто виртуальный метод `PutsShip()`, т.е. объект этого класса нельзя создать, но в нем есть все те методы, которые должны быть у всех игроков.\
Первое поле − вектор всех кораблей, что есть у игрока. Ссылки этих объектов будет содержать в себе поле `p_board[]` в классе `board`. 
Также создание вектора кораблей позволяет игроку выбирать, какой корабль он хочет поставить: однопалубный, двухпалубный и т.д.
|Поле класса _Player_|Тип|Описание поля|
|:-|:------|:------|
|m_ships|std::vector\<int\>|Вектор кораблей|
|m_board|board|Поле для игры|
|m_shipCount[ ]|int|Количество кораблей|

Второе поле − игровое поле, который должен иметь каждый игрок.\
Третье поле − массив целых чисел, обозначающих количество кораблей. Первый элемент массива − количество однопалубных кораблей, второй − количество двухпалубных и т.д.
Если сумма элементов этого массива будет равна нулю, значит, другой игрок победил и игра окончена (либо все корабли расставлены, и можно начинать игру).
<a name="human"><h3>Класс _Human_</h3></a>
Производный класс от абстрактного класса `Player`. Обозначает реального человека в игре. У него нет своих полей, т.к. все нужные поля объявлены в базовом классе, 
но есть определенный метод `PutsShip()`. В этом методе реализован алгоритм расставления кораблей человеком: 
человек поочередно вводит номер корабля и координаты с направлением, каждый ввод проверяется на ошибки, и затем, если ошибок нет, ставится корабль. 
При вводе номера корабля можно ввести _back_, чтобы удалить последний поставленный корабль, а при вводе координат (когда корабль выбран) можно снова ввести _back_, тогда отменится выбор корабля.
<a name="bot"><h3>Класс _Bot_</h3></a>
Производный класс от абстрактного класса `Player`. Обозначает виртуального игрока в игре (бота). У него нет своих полей, т.к. все нужные поля объявлены в базовом классе, 
но есть определенный метод `PutsShip()`, который случайно расставляет корабли: сначала случайно выбирает корабль и одно из четырех (или возможных) направлений, а затем ставит корабль.
<a name="seab"><h3>Класс _SeaBattle_</h3></a>
Этот класс представляет саму игру "Морской Бой". Именно объект этого класса создается в главной функции `main()`, и вызывается единственный доступный метод класса `Start()` 
(который является неким аналогом `main()` в этом классе). Этот класс имеет два поля − двух игроков.
|Поле класса _SeaBattle_|Тип|Описание поля|
|:-|:------|:------|
|m_human|Human|Игрок №1 − человек|
|m_bot|Bot|Игрок №2 − бот|

Один из методов класса − функция `HumanMove()` − ход человека. Человек вводит координаты точки предполагаемого корабля, ввод проверяется на ошибки, и затем, если ошибок нет, 
проверяется: есть ли там корабль, если есть − ставится определенный символ (`S` − корабль убит, `V` − корабль подбит), и затем функция начинается заново до первого промаха (символ `Х`), либо до конца игры (вот это везение).\
Другой метод класса − `BotMove()` − ход компьютера. На данный момент точка выбирается случайно и равновероятно до первого попадания, затем компьютер пытается найти корабль, исходя из этой точки (и следующих).
<a name="cons"><h3>Класс _Console_</h3></a>
Этот класс содержит в себе некоторые статические функции для работы с консолью: очистить вывод, приостановить выполнение программы и т.д.\
Также в ней осуществляется работа со вводом: человек может ввести строку с пробелом, тогда символы после пробела останутся в потоке и пойдут на следующий ввод, поэтому их надо удалять.

---
<a name="todo"><h2>Обзор игры</h2></a>
Вся логика игры отображена в функции `Start()` класса `SeaBattle`. 
Запуск игры начинается с правил, касающихся ввода: что вводить и как вводить (не правил самой игры): \
\
![тут правила](https://github.com/bukmanduk/SeaBattle/blob/master/assets/rules.PNG "Скучные правила")
После того, как игрок введет `1` (хотя ввести можно вообще все), ему предложат разместить корабли на поле (добровольно-принудительно).\
Сверху отображается информация о кораблях: номер, количество палуб (совпадает с номером) и количество кораблей.\
Игрок сначала вводит номер корабля:\
\
![ставим корабли](https://github.com/bukmanduk/SeaBattle/blob/master/assets/putsship.PNG "Начинаем расставлять корабли")
А затем координаты точки корабля − номер строки и название столбца:\
\
![ставим корабли](https://github.com/bukmanduk/SeaBattle/blob/master/assets/putsships2.PNG "Вводим, куда поставить: ставлю на 1a>")
Каждый ввод проверяется на ошибки. Так, например, нельзя выбрать корабль, если его количество равно нулю, или не получится разместить корабль, если он будет выходить за пределы поля и/или пересекать другой корабль. \
Когда игрок расставит все корабли, начнется игра.\
Поочередно, начиная с игрока, игрок и компьютер делают ход, пока количество их кораблей не равно нулю.\
Пример старта игры (нулевого хода):\
\
![а тут игра](https://github.com/bukmanduk/SeaBattle/blob/master/assets/game.PNG "Ура, играем!")
Пример некоторого хода:\
\
![еще игра](https://github.com/bukmanduk/SeaBattle/blob/master/assets/game2.PNG "Напряженный момент")
Сверху обозначается количество имеющихся неуничтоженных кораблей. В этой игре на некотором ходе (картинка сверху) мне осталось найти один однопалубный корабль, а компьютеру два однопалубных.\
Когда у одного из игроков не останется на поле неуничтоженных кораблей, игра заканчивается: выводится победитель и, если победил компьютер, выводится измененное поле бота − отображаются ненайденные корабли.\
В этой игре победила я, но были тесты, где побеждал и компьютер (но все же реже).\
\
`Game Over`
\
\
__Скачать и протестировать игру --->__ [`Download`](https://github.com/bukmanduk/SeaBattle/releases/download/v1.0.0/Battleship.exe)
