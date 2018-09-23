import React, { Component } from "react";
import Field from "./field";

//this class takes an array of objects denoting input fields and their attributes and prints them
//input: body object
//output: list of input fields and their names
class FieldList extends Component {
	state = {};
	render() {
		if (this.validateProps()) {
			//if body is nonempty
			if (this.props.body) {
				return this.props.body.map(atr => (
					<Field key={atr.name} attributes={atr} /> //create a Field for each index of body
				));
			}
			return <h2>Empty</h2>; //if the body is empty, display this so we know there was no errors
		}
		return <h2>Error initializing FieldList component props</h2>;
	}

	validateProps() {
		if (this.props.body) {
			if (typeof this.props.body !== "object") {
				return false;
			}
		}
		return true;
	}
}

export default FieldList;
