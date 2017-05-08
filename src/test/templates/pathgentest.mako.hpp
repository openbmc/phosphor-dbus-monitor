const std::array<std::string, ${len(meta)}> meta = {
% for m in meta:
    "${m.name}"s,
% endfor
};

const std::array<std::string, ${len(paths)}> paths = {
% for p in paths:
    "${p.name}"s,
% endfor
};

const std::array<PathMeta, ${len(pathmeta)}> pathMeta = {{
% for p in pathmeta:
    { std::cref(paths[${p.path}]), std::cref(meta[${p.meta}]) },
% endfor
}};

const std::array<RefVector<const std::string>, ${len(pathgroups)}> groups = {{
% for g in pathgroups:
    // ${g.name}
    {
        % for p in g.members:
        std::cref(paths[${p[0]}]),
        % endfor
    },
% endfor
}};
