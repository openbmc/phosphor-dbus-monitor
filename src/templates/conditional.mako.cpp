std::make_unique<ConditionalCallback<ConfigPropertyCallbacks>>(
${indent(1)}ConfigPropertyCallbackGroups::get()[${c.graph}],
${indent(1)}*ConfigConditions::get()[${c.condition}])\
