type result 'a = Js.t {.
    body: Js.null (Js.t {..} as 'a),
    clientError: Js.boolean,
    info: Js.boolean,
    notFound: Js.boolean,
    ok: Js.boolean,
    serverError: Js.boolean,
    status: int,
    statusCode: int,
    statusText: string,
    statusType: int,
    text: string,

    error: Falsy.t (Js.t {.
        method: string,
        status: int,
        url: string,
        message: string,
        stack: string
    })
};

module Request(M: { type t; }) => {
    external withCredentials : M.t => M.t = "" [@@bs.send];

    external _end :
        M.t => (Js.undefined string => Js.undefined (result 'a) => unit)
        => unit = "end" [@@bs.send];

    let end_ req => {
        Js.Promise.make @@ fun ::resolve reject::_ => {
            _end req (fun err result => {
                resolve (Js.Undefined.to_opt err, Js.Undefined.to_opt result) [@bs];
            })
        };
    };
};

module Post = {
    type t;
    type t2 = t;

    include Request({ type t = t2; });
};

module Get = {
    type t;
    type t2 = t;

    include Request({ type t = t2; });
};

external get : string => Get.t = "" [@@bs.module "superagent"];
external post : string => Post.t = "" [@@bs.module "superagent"];