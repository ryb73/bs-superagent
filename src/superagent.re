type result = Js.t {.
    body: Js.nullable Js.Json.t,
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
    external query : Js.t {..} => M.t = "" [@@bs.send.pipe: M.t];

    external _end :
        M.t => (Js.nullable string => Js.nullable result => unit)
        => unit = "end" [@@bs.send];

    let end_ req => {
        Js.Promise.make @@ fun ::resolve reject::_ => {
            _end req (fun err result => {
                resolve (Js.Nullable.to_opt err, Js.Nullable.to_opt result) [@bs];
            })
        };
    };
};

module Post = {
    type t;
    type t2 = t;

    include Request({ type t = t2; });

    external send : Js.t {..} => t = "" [@@bs.send.pipe: t];
};

module Get = {
    type t;
    type t2 = t;

    include Request({ type t = t2; });
};

external get : string => Get.t = "" [@@bs.module "superagent"];
external post : string => Post.t = "" [@@bs.module "superagent"];