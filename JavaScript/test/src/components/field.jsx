import React, { Component } from "react";

class Field extends Component {
	state = {
		label: ""
	};
	render() {
		this.generateLabel();
		return (
			<div>
				<label htmlFor="email">{this.state.label}</label>
				<br />
				<input {...this.props.attributes} />
				<br />
			</div>
		);
	}

	generateLabel() {
		let lower = this.props.attributes.name;
		let upper = lower.charAt(0).toUpperCase() + lower.substr(1);
		this.state.label = upper.replace("-", " ");
	}
}

export default Field;
