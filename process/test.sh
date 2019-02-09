#!/bin/sh
echo "you are in test mod"

inputPath="./input_test"
outputPath="./output_test"

#遍历文件
for file in `ls ${inputPath}`
    do
    echo "————————————————————————————————"${file}
    for line in `cat ${inputPath}/${file}`
        do
        sed -i '1d' ./input.txt
        # echo $line
        echo $line >> ./input.txt
    done
    #执行文件
    # make
    ./multisum
    #比较结果
    output=$(cat output.txt)
    output_test=$(cat ${outputPath}/${file})
    echo "测试结果:"${output}
    echo "测试用例给出结果:"${output_test}
    if [ $output -eq $output_test ]
        then
            echo "——————————————————————————————————测试正确"
        else
            echo "——————————————————————————————————测试错误"
    fi
done