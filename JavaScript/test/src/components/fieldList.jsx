import React, { Component } from "react";
import Field from "./field";

class FieldList extends Component {
	state = {};
	render() {
		if (this.props.body) {
			let fields = [];
			for (let i = 0; i < this.props.body.length; i++) {
				fields.push(<Field key={i} attributes={this.props.body[i]} />);
			}
			return fields;
		}
		return <h1>Empty</h1>;
	}
}

export default FieldList;
