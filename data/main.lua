print("hello world!")

function main(...)
    for i = 1, arg.n do
        print(arg[i])
    end
    print(get_current_dir())
end
