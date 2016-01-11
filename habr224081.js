// http://habrahabr.ru/post/224081/
// fixed xml parser

var digit = {
    exec: function (str, pos) {
        var chr = str.charAt(pos);
        if (chr >= "0" && chr <= "9") 
            return { res: +chr, end: pos + 1};
    }
};
function Pattern(zexec) {
    this.exec = zexec;
    this.then = function (transform) {
        return new Pattern(function (str, pos) {
            var r = zexec(str, pos);
            return r && { res: transform(r.res), end: r.end };
        });
    };
    this.exec = function (str, pos) {
        var r = zexec(str, pos || 0);
        return pos >= 0 ? r : !r ? null : r.end != str.length ? null : r.res;
    };
}
function txt(text) {
    return new Pattern(function (str, pos) {
        if (str.substr(pos, text.length) == text)
            return { res: text, end: pos + text.length };
    });
}
function rgx(regexp) {
    return new Pattern(function (str, pos) {
        var m = regexp.exec(str.slice(pos));
        if (m && m.index === 0)
            return { res: m[0], end: pos + m[0].length };
    });
}
function opt(pattern) {
    return new Pattern(function (str, pos) {
        return pattern.exec(str, pos) || { res: void 0, end: pos };
    });
}
function exc(pattern, except) {
    return new Pattern(function (str, pos) {
        return !except.exec(str, pos) && pattern.exec(str, pos);
    });
}
function any(...patterns) {
    return new Pattern(function (str, pos) {
        for (var r, i = 0; i < patterns.length; i++)
            if (r = patterns[i].exec(str, pos))
                return r;
    });
}
function seq(...patterns) {
    return new Pattern(function (str, pos) {
        var i, r, end = pos, res = [];

        for (i = 0; i < patterns.length; i++) {
            r = patterns[i].exec(str, end);
            if (!r) return;
            res.push(r.res);
            end = r.end;
        }

        return { res: res, end: end };
    });
}
function rep(pattern, separator) {
    var separated = !separator ? pattern :
        seq(separator, pattern).then(r => r[1]);

    return new Pattern(function (str, pos) {
        var res = [], end = pos, r = pattern.exec(str, end);

        while (r && r.end > end) {
            res.push(r.res);
            end = r.end;
            r = separated.exec(str, end);
        }

        return { res: res, end: end };
    });
}
var name = rgx(/[a-z]+/i).then(s => s.toLowerCase());
var char = rgx(/[^"&<]/i);
var quoted = seq(txt('"'), rep(char), txt('"')).then(r => r[1].join(''));
var attr = seq(name, txt('='), quoted).then(r => ({ name: r[0], value: r[2] }));

var wsp = rgx(/\s+/);

var attrs = rep(attr, wsp).then(r => {
    var m = {}; 
    r.forEach(a => (m[a.name] = a.value));
    return m;
});

var header = seq(txt('<?xml'), wsp, attrs, txt('?>')).then(r => r[2]);

var text = rep(char).then(r => r.join(''));
var subnode = new Pattern(function(str, pos){return node.exec(str, pos)});

var rawnode = seq(
    txt('<'), name, opt(wsp), opt(attrs), txt('>'),
    rep(any(subnode, text), opt(wsp))
    ,txt('</'), name, txt('>')
).then(r => ({ tag: r[1], attrs: r[3], nodes: r[5] }));
var node=rawnode;//seq(opt(wsp),rawnode);
//var xml = (header + node).then(r => ({ root: r[1], attrs: r[0] }));
var out=node.exec(POST['data'],0);
//var out=rep(rgx(/\d+/), txt(",")).exec(POST['data'],0);
return JSON.stringify(out,null,"  ");
