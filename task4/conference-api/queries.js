const database = db.getSiblingDB("conference_mongo");

print("\n=== CREATE operations ===");

const tempUserId = ObjectId("660000000000000000000099");
const tempTalkId = ObjectId("770000000000000000000099");
const conferenceId = ObjectId("880000000000000000000001");

database.conferences.updateMany({}, { $pull: { talks: { talk_id: tempTalkId } } });
database.tokens.deleteMany({ user_id: tempUserId });
database.tokens.deleteMany({ token: "token_temporary_user" });
database.talks.deleteOne({ _id: tempTalkId });
database.users.deleteOne({ _id: tempUserId });

database.users.insertOne({
  _id: tempUserId,
  login: "temporary_user",
  password_hash: "hash_temporary",
  first_name: "Временный",
  last_name: "Пользователь",
  role: "speaker",
  email: "temporary@example.com",
  bio: "Временный пользователь для демонстрации CRUD операций",
  contacts: {
    telegram: "@temporary",
    github: "temporary-user"
  },
  interests: ["mongodb", "crud"],
  rating: 4.1,
  created_at: new Date(),
  updated_at: new Date()
});

print("Created user:");
printjson(database.users.findOne({ _id: tempUserId }));

database.talks.insertOne({
  _id: tempTalkId,
  title: "Временный доклад по MongoDB CRUD",
  description: "Доклад создан для демонстрации операций insert, update и delete",
  speaker_id: tempUserId,
  status: "submitted",
  level: "beginner",
  duration_minutes: 30,
  tags: ["mongodb", "crud", "demo"],
  materials: [],
  created_at: new Date(),
  updated_at: new Date()
});

print("Created talk:");
printjson(database.talks.findOne({ _id: tempTalkId }));

database.tokens.insertOne({
  token: "token_temporary_user",
  user_id: tempUserId,
  created_at: new Date(),
  expires_at: new Date(Date.now() + 24 * 60 * 60 * 1000),
  user_agent: "mongosh",
  active: true
});

print("Created token:");
printjson(database.tokens.findOne({ user_id: tempUserId }));

print("\n=== READ operations ===");

print("\nUsers with role equal to speaker ($eq):");
printjson(
  database.users
    .find(
      { role: { $eq: "speaker" } },
      { login: 1, role: 1, rating: 1 }
    )
    .toArray()
);

print("\nUsers whose role is not participant ($ne):");
printjson(
  database.users
    .find(
      { role: { $ne: "participant" } },
      { login: 1, role: 1 }
    )
    .toArray()
);

print("\nTalks longer than 45 minutes ($gt):");
printjson(
  database.talks
    .find(
      { duration_minutes: { $gt: 45 } },
      { title: 1, duration_minutes: 1 }
    )
    .toArray()
);

print("\nConferences with capacity less than 200 ($lt):");
printjson(
  database.conferences
    .find(
      { capacity: { $lt: 200 } },
      { title: 1, capacity: 1 }
    )
    .toArray()
);

print("\nTalks with status accepted or submitted ($in):");
printjson(
  database.talks
    .find(
      { status: { $in: ["accepted", "submitted"] } },
      { title: 1, status: 1 }
    )
    .toArray()
);

print("\nPaid conferences with capacity greater than 100 ($and):");
printjson(
  database.conferences
    .find(
      {
        $and: [
          { paid: true },
          { capacity: { $gt: 100 } }
        ]
      },
      { title: 1, paid: 1, capacity: 1, price: 1 }
    )
    .toArray()
);

print("\nUsers who are organizers or have rating greater than 4.8 ($or):");
printjson(
  database.users
    .find(
      {
        $or: [
          { role: "organizer" },
          { rating: { $gt: 4.8 } }
        ]
      },
      { login: 1, role: 1, rating: 1 }
    )
    .toArray()
);

print("\nOnline conferences by embedded field location.online:");
printjson(
  database.conferences
    .find(
      { "location.online": true },
      { title: 1, location: 1 }
    )
    .toArray()
);

print("\nTalks with mongodb tag:");
printjson(
  database.talks
    .find(
      { tags: "mongodb" },
      { title: 1, tags: 1 }
    )
    .toArray()
);

print("\n=== UPDATE operations ===");

database.users.updateOne(
  { _id: tempUserId },
  {
    $set: {
      bio: "Обновлённое описание временного пользователя",
      rating: 4.5,
      updated_at: new Date()
    }
  }
);

print("\nUpdated user with $set:");
printjson(database.users.findOne({ _id: tempUserId }));

database.talks.updateOne(
  { _id: tempTalkId },
  {
    $push: {
      materials: {
        type: "slides",
        url: "https://example.com/slides/temp-mongodb-crud.pdf"
      }
    },
    $set: {
      updated_at: new Date()
    }
  }
);

print("\nUpdated talk with $push to materials:");
printjson(database.talks.findOne({ _id: tempTalkId }));

database.talks.updateOne(
  { _id: tempTalkId },
  {
    $addToSet: {
      tags: "laboratory"
    },
    $set: {
      updated_at: new Date()
    }
  }
);

print("\nUpdated talk with $addToSet to tags:");
printjson(database.talks.findOne({ _id: tempTalkId }));

database.conferences.updateOne(
  { _id: conferenceId },
  {
    $addToSet: {
      talks: {
        talk_id: tempTalkId,
        order: 99,
        room: "Demo Room",
        starts_at: new Date("2026-06-01T18:00:00Z"),
        status: "accepted"
      }
    },
    $set: {
      updated_at: new Date()
    }
  }
);

print("\nConference after $addToSet temporary talk:");
printjson(
  database.conferences.findOne(
    { _id: conferenceId },
    { title: 1, talks: 1 }
  )
);

database.talks.updateOne(
  { _id: tempTalkId },
  {
    $pull: {
      materials: {
        type: "slides"
      }
    },
    $set: {
      updated_at: new Date()
    }
  }
);

print("\nUpdated talk with $pull from materials:");
printjson(database.talks.findOne({ _id: tempTalkId }));

database.conferences.updateOne(
  { _id: conferenceId },
  {
    $pull: {
      talks: {
        talk_id: tempTalkId
      }
    },
    $set: {
      updated_at: new Date()
    }
  }
);

print("\nConference after $pull temporary talk:");
printjson(
  database.conferences.findOne(
    { _id: conferenceId },
    { title: 1, talks: 1 }
  )
);

print("\n=== DELETE operations ===");

const deletedTokens = database.tokens.deleteMany({ user_id: tempUserId });
print("Deleted tokens count: " + deletedTokens.deletedCount);

const deletedTalk = database.talks.deleteOne({ _id: tempTalkId });
print("Deleted talks count: " + deletedTalk.deletedCount);

const deletedUser = database.users.deleteOne({ _id: tempUserId });
print("Deleted users count: " + deletedUser.deletedCount);

print("\n=== Aggregation pipeline ===");

print("\nTalk statistics by status:");
printjson(
  database.talks.aggregate([
    {
      $match: {
        duration_minutes: { $gt: 0 }
      }
    },
    {
      $group: {
        _id: "$status",
        talks_count: { $sum: 1 },
        average_duration: { $avg: "$duration_minutes" },
        max_duration: { $max: "$duration_minutes" }
      }
    },
    {
      $project: {
        _id: 0,
        status: "$_id",
        talks_count: 1,
        average_duration: { $round: ["$average_duration", 2] },
        max_duration: 1
      }
    },
    {
      $sort: {
        talks_count: -1
      }
    }
  ]).toArray()
);

print("\nCRUD queries completed successfully.");
