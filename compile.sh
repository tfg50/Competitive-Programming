obj="x"
while getopts o:pct flag
do
    case "${flag}" in
        o) obj=${OPTARG};;
        p) ;;
        c) ;;
        t)
        cmd="g++ $obj.cpp -o $obj -I. -Wshadow -Wall -Wconversion -O2 -std=c++20 -DFAST_COMPILATION -DTEST"
        # cmd2="g++ testing/generate.cpp -o generator -I. -Wshadow -Wall -Wconversion -O2 -std=c++20 -DFAST_COMPILATION -DTEST"
        echo $cmd
        if $cmd #&& $cmd2
        then
            echo "Running python generation"
            python3 testing/generate.py > "auto_in"
            echo "Running solution"
            ./$obj < "auto_in" > "auto_out"
            echo "Ended"
            exit 0
        else
            echo "Failed somehow"
            exit 1
        fi
        ;;
    esac
done
if [[ $* == *-p* ]] || [[ $* != *-c* ]]
then
    python3 Biblioteca/precompiler.py
    echo "Precompiling into clipboard"
fi
if [[ $* != *-p* ]] || [[ $* == *-c* ]]
then
    cmd="g++ $obj.cpp -o $obj -I. -Wshadow -Wall -Wconversion -O2 -std=c++20 -DFAST_COMPILATION"
    echo $cmd
    if $cmd
    then
        echo "Compilation done"
        exit 0
    else
        echo "Compilation failed"
        exit 1
    fi
fi

# wait