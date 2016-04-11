json.array!(@questions) do |question|
  json.extract! question, :id, :question, :answer, :category
  json.url question_url(question, format: :json)
end
