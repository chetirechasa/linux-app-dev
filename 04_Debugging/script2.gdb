# Запуск программы с параметрами -100 100 3
set pagination off
set verbose off

# Счетчик элементов
set $counter = 0

# Точка останова с условием по счетчику
break range.c:54
commands
    set $counter = $counter + 1
    if ($counter >= 28 && $counter <= 35)
        echo @@@
        print start
        echo @@@
        print stop
        echo @@@
        print step
        echo @@@
        print i
        echo @@@\n
    end
    continue
end

# Запускаем программу
run -100 100 3 > /dev/null

quit

