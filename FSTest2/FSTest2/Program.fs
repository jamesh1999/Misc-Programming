// Learn more about F# at http://fsharp.org
// See the 'F# Tutorial' project for more help.
open System

type MorseChar = 
    | Dot
    | Dash

type InputChar =
    {Code: MorseChar list;
    Letter: char}

type Node =
    | Leaf of char
    | Branch of char option * Node * Node

let updateTrie chr =
    //Sorts by dot/dash code
    let sort =
        let rec iter x y =

            let cmp x y =
                match x with
                | a when a = y -> 0
                | Dot -> 1
                | Dash -> -1

            match (x, y) with
            | (xh::xt, yh::yt) ->
                //Compare current char
                match cmp xh yh with
                | 0 -> iter xt yt
                | a -> a
            | _ ->
                //Compare length
                if x.Length = y.Length then 0 elif x.Length > y.Length then 1 else -1

        iter

    let rec makeTrie l =
        let depth = (l.Item 0).Code.Length

        let fold acc x =
            

        List.fold fold 

    List.sortWith sort l
    |> makeTrie

let input =
   [
   { Code = [Dot; Dash]; Letter = 'A'};
   { Code = [Dash; Dot; Dot; Dot]; Letter = 'B'};
   { Code = [Dot; Dot; Dot]; Letter = 'C'};
   { Code = [Dash; Dot; Dot]; Letter = 'D'};
   { Code = [Dot]; Letter = 'E'};
   ]
            

[<EntryPoint>]
let main argv = 
    printfn "%A" argv
    0 // return an integer exit code
