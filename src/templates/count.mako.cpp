std::make_unique<CountCondition<${c.datatype}>>(
${indent(1)}ConfigPropertyIndicies::get()[${c.instances}],
${indent(1)}[](const auto& item){return item ${c.countop} ${c.countbound};},
${indent(1)}[](const auto& item){return item ${c.op} ${c.bound.argument(loader, indent=indent +1)};})\
