import React, { Component } from "react";
import Field from "./field";

class FieldList extends Component {
	state = {};
	render() {
		//if body is nonempty
		if (this.props.body) {
			return this.props.body.map(atr => (
				<Field key={atr.name} attributes={atr} /> //create a Field for each index of body
			));
		}
		return <h2>Empty</h2>; //if the body is empty, display this so we know there was no errors
	}
}

export default FieldList;
