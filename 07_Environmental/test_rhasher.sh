#!/bin/bash
echo "Running basic tests..."

# Простой тест MD5 строки
result=$(echo 'MD5 "test"' | ./rhasher 2>/dev/null | tr -d ' \n')
expected=$(echo -n "test" | md5sum | cut -d' ' -f1)

if [ "$result" = "$expected" ]; then
    echo "PASS: Basic MD5 test"
else
    echo "FAIL: Basic MD5 test"
    echo "Expected: $expected"
    echo "Got: $result"
fi

# Тест на ошибку
echo "UNKNOWN test" | ./rhasher 2>&1 | grep -q "Error"
if [ $? -eq 0 ]; then
    echo "PASS: Error handling"
else
    echo "FAIL: Error handling"
fi

