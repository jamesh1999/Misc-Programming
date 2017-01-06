// Learn more about F# at http://fsharp.org
// See the 'F# Tutorial' project for more help.

open System

let f x =
    match (x % 3, x % 5) with
    | (0, 0) -> printfn "FizzBuzz"
    | (0, _) -> printfn "Fizz"
    | (_, 0) -> printfn "Buzz"
    | (_, _) -> printfn "%i" x


[<EntryPoint>]
let main argv = 
    List.iter f [1..102]
    System.Console.Read() |> ignore
    0
