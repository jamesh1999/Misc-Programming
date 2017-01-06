//Enter your code here. Read input from STDIN. Print output to STDOUT
open System

    
    
//Vector operations
let mag (v: float list) =
    Math.Sqrt(Math.Pow(List.item 0 v, 2.0) + Math.Pow(List.item 1 v, 2.0))

let normalize (v: float list) =
    List.map (fun x -> x / (mag v)) v
        
let getV (p1: float list) (p2: float list) =
    List.zip p1 p2
    |> List.map (fun (x,y) -> x - y)
        
let dot (v1: float list) (v2: float list) =
    List.zip v1 v2
    |> List.fold (fun acc (x,y) -> acc + x*y) 0.0

let angleTo (v1: float list) (v2: float list) =
    let a = List.item 0 v2
    let b = List.item 1 v2
    let c = List.item 0 v1
    2.0*Math.Atan((-b-Math.Sqrt(a**2.0+b**2.0-c**2.0))/(a+c))
    
let getTriArea (p1: float list) (p2: float list) (p3: float list) =
    let v1 =
        getV p1 p2
        |> normalize
    let v2 =
        getV p1 p3
        |> normalize
    
    //Calculate 0.5*a*b*sinC
    let c = Math.Sin(Math.Acos(dot v1 v2))
    0.5 * c * (getV p1 p2 |> mag) * (getV p1 p3 |> mag)
    
let filterIndex =
    let rec g n f (l: float list list) =
        match n with
        | x when x = l.Length -> []
        | _ ->
            match f n with
            | true -> (List.item n l)::(g (n + 1) f l)
            | false -> g (n + 1) f l
    g 0

let listRemove x (l: float list list) =
    filterIndex (fun y -> y = x) l
        
    
let getPolyArea (coords: float list list) =
    let wrap x y =
        match x % y with
        | z when z < 0 -> z + y
        | z -> z
        
    //Get winding direction
    let dir =
        let rec f n (l: float list list) =    
            match n with
            | x when x < l.Length ->
                let v1 = getV (List.item n l) (List.item (wrap (n - 1) l.Length) l)
                let v2 = getV (List.item n l) (List.item (wrap (n + 1) l.Length) l)
                (angleTo v1 v2) + (f (n + 1) l)
            | _ -> 0.0
        (f 0 coords) > 0.0
        
    let nextTri =
        let rec iter n l =
            let v1 = getV (List.item n l) (List.item (wrap (n - 1) l.Length) l)
            let v2 = getV (List.item n l) (List.item (wrap (n + 1) l.Length) l)
            
            match angleTo v1 v2 with
            | x when (x > 0.0 && dir) -> n
            | x when (x < 0.0 && not dir) -> n
            | _ -> iter (n + 1) l
        iter 0
    
    let rec g (l: float list list) =
        match l.Length with
        | x when x < 3 -> 0.0
        | _ ->
            let n = nextTri l
            (g (listRemove n l)) + (getTriArea (List.item n l) (List.item (wrap (n - 1) l.Length) l) (List.item (wrap (n + 1) l.Length) l))
        
    g coords

[<EntryPoint>]
let main argv = 
    Console.ReadLine() |> ignore

    let parseCoord (x: string) =
        x.Split([|' '|])
        |> Seq.toList
        |> List.map Double.Parse

    let input =
        Seq.initInfinite (fun _ -> Console.ReadLine())
        |> Seq.takeWhile (fun x -> x <> null)
        |> Seq.toList
        |> List.map parseCoord

    Console.WriteLine(getPolyArea input)
    Console.ReadLine() |> ignore

    0 // return an integer exit code
