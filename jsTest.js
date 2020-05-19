function removePadding(value, padding, leftPad = true)
{
    if(leftPad)
    {
       let i;
        for(i = value.length; i >= 0; i--)
        {
            if(value[i] == padding)
            {
                break;
            }
        }
        let length = value.length - i;
        //console.log(length);
        return value.substring(value.length - length, value.length);
    }
     let i;
    for(i = 0; i < value.length; i++)
    {
        if(value[i] == padding)
        {
            break;
        }
    }
    return value.substring(0, i);
}

let DEFUALT_DIR_SIZE = 10;
removePadding("fil.txt/////////////////////////", "/", false);

function parse(){
    let data = `fil.txt/////////////////////////00000000230015898326270015898326271hejsa, dette er en fil
mappe///////////////////////////00000005600015898326330015898326332////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003innerFil.txt////////////////////00000000240015898326450015898326451denne tekst er hemmelig
�////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003////////////////////////////////00000000000000000000000000000000003`;

    while(data.length > 0)
    {    
        for(let i = 0; i < DEFUALT_DIR_SIZE; i++)
        {

            let name = removePadding(data.substring(0, 32), "/", false);
            data = data.substring(32);

            let size = Number(data.substring(0, 10));//removePadding(data.substring(0, 10), "0", true);
            data = data.substring(10);

            let modTime = removePadding(data.substring(0, 12), "0", true);
            data = data.substring(12);

            let accessTime = removePadding(data.substring(0, 12), "0", true);
            data = data.substring(12);

            let type = data.substring(0, 1);
            data = data.substring(1);
            
            let out = {
                name,
                size,
                modTime,
                accessTime,
                type,
            };

            if(type == "1")
            {
                console.log("IS_FILE!!");
                let fileContent = data.substring(0, size);
                data = data.substring(size);

                out.data = fileContent;
            }

            console.log(out);
            console.log("---");
        }
        console.log("-------- NEW FOLDER -------");
    }
}

parse();