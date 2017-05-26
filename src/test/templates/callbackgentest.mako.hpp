auto storageCount = ${len(instances)};

const std::array<Index, ${len(instancegroups)}> indicies = {{
% for g in instancegroups:
    {
    % for i in g.members:
        {{${i[0]}, ${i[2]}, ${i[3]}}, ${i[5]}},
    % endfor
    },
% endfor
}};

const std::array<std::tuple<std::string, size_t>, ${len(callbacks)}> callbacks = {{
% for c in callbacks:
    {"${c.datatype}", ${c.instances}},
% endfor
}};
