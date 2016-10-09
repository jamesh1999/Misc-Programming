// Learn more about F# at http://fsharp.org
// See the 'F# Tutorial' project for more help.

open System

[<EntryPoint>]
let main argv = 

    let rows = [ (1,"a"); (-22,"bb"); (333,"ccc"); (-4444,"dddd") ] 

    // no alignment
    for (i,s) in rows do
        printfn "|%i|%s|" i s

    // with alignment
    for (i,s) in rows do
        printfn "|%5i|%5s|" i s

    // with left alignment for column 2
    for (i,s) in rows do
        printfn "|%5i|%-5s|" i s

    // with dynamic column width=20 for column 1
    for (i,s) in rows do
        printfn "|%*i|%-5s|" 20 i s 

    // with dynamic column width for column 1 and column 2
    for (i,s) in rows do
        printfn "|%*i|%-*s|" 20 i 10 s 

    System.Console.ReadLine() |> ignore
    0 // return an integer exit code
