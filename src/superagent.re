open Js.Result;

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

type contentType =
  | ApplicationJson;

type authorizationType =
  | Bearer;

type header =
  | ContentType contentType
  | Accept contentType
  | Authorization authorizationType string;

module Request(M: { type t; }) => {
    external withCredentials : M.t => M.t = "" [@@bs.send];

    external query : Js.Dict.t string => M.t = "" [@@bs.send.pipe: M.t];

    external _end :
        M.t => (Js.nullable string => Js.nullable result => unit)
        => unit = "end" [@@bs.send];

    let end_ req => {
        Js.Promise.make @@ fun ::resolve reject::_ => {
            _end req (fun err resp => {
                switch (Js.Nullable.to_opt err) {
                    | Some err => resolve (Error err) [@bs]
                    | None => switch (Js.Nullable.to_opt resp) {
                        | None => resolve (Error "No Response") [@bs]
                        | Some resp => resolve (Ok resp) [@bs]
                    };
                };
            })
        };
    };

    external setHeaderCustom : string => string => M.t = "set" [@@bs.send.pipe: M.t];

    let setHeader header req => {
        switch header {
            | ContentType v => {
                let key = "Content-Type";
                switch v {
                    | ApplicationJson => setHeaderCustom key "application/json" req
                };
            }

            | Accept v => {
                let key = "Accept";
                switch v {
                    | ApplicationJson => setHeaderCustom key "application/json" req
                };
            }

            | Authorization authType credentials => {
                let key = "Authorization";
                switch authType {
                    | Bearer => setHeaderCustom key ("Bearer " ^ credentials) req
                };
            }
        };
    };
};

module Post = {
    type t;

    include Request({ type nonrec t = t; });

    external _send : string => t = "send" [@@bs.send.pipe: t];
    let send json req => {
        req
            |> setHeader (ContentType ApplicationJson)
            |> _send @@ Js.Json.stringify json;
    };
};

module Get = {
    type t;

    include Request({ type nonrec t = t; });
};

external get : string => Get.t = "" [@@bs.module "superagent"];
external post : string => Post.t = "" [@@bs.module "superagent"];